#include "Renderer.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cmath>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Primitives/Scene.h"
#include "../model.h"
static enum class RenderPass {
    NONE = 0,
    SKYBOX,
    DEFAULT,
    PBR
};
const int num_passes = static_cast<int>(RenderPass::DEFAULT) ;

Texture* colourAttachment;
glm::vec3 lightPositions[] = {
    glm::vec3(-10.0f, 10.0f, 10.0f),
    glm::vec3(10.0f, 10.0f, 10.0f),

    glm::vec3(-10.0f, -10.0f, 10.0f),
    glm::vec3(10.0f, -10.0f, 10.0f)
};

glm::vec3 lightColors[] = {
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f)
};

int nrRows = 7;
int nrColumns = 7;
float spacing = 2.5;
unsigned int albedo;
unsigned int normal;
unsigned int metallic;
unsigned int roughness;
unsigned int ao;
void Renderer::Setup()
{
    // Create ScreenVAO
    glGenVertexArrays(1, &ScreenVAO);
    glBindVertexArray(ScreenVAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, ScreenVertices.size() * sizeof(float), &ScreenVertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);

    // Enable features
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_MULTISAMPLE);

    // Load shaders 
    shaders[static_cast<int>(RenderPass::SKYBOX)] = Shader();
    shaders[static_cast<int>(RenderPass::SKYBOX)].Load("../Shaders/skybox.vs", "../Shaders/Cubemap.fs");
    shaders[static_cast<int>(RenderPass::DEFAULT)] = Shader();
    shaders[static_cast<int>(RenderPass::DEFAULT)].Load("../Shaders/shader.vs", "../Shaders/shader.fs");
    shaders[static_cast<int>(RenderPass::PBR)] = Shader();
    shaders[static_cast<int>(RenderPass::PBR)].Load("../Shaders/pbr.vs", "../Shaders/pbr.fs");


    albedo = TextureFromFile("../Textures/rusted_iron/albedo.png", ".");
    normal = TextureFromFile("../Textures/rusted_iron/normal.png", ".");
    metallic = TextureFromFile("../Textures/rusted_iron/metallic.png", ".");
    roughness = TextureFromFile("../Textures/rusted_iron/roughness.png", ".");
    ao = TextureFromFile("../Textures/rusted_iron/ao.png",".");


}

void Renderer::resize(int width, int height ) {
    framebuffer = FrameBuffer();
    
    framebuffer.Bind();

    ColourBuffer = CreateTexture(width, height);

    framebuffer.Attach(*ColourBuffer);

    renderbufferObject = RenderBuffer();

    renderbufferObject.Bind();

    renderbufferObject.UseDepthAndStencil(width, height);

    framebuffer.Attach(renderbufferObject);

    if (framebuffer.IsComplete() == false) {
        std::cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete! " << std::endl;
    }

    renderbufferObject.Unbind();

    OutlineEffect = FrameBuffer();
    OutlineEffect.Bind();

    colourAttachment = CreateTexture(width, height);
    OutlineEffect.Attach(*colourAttachment);

    auto renderBuffer = RenderBuffer();
    renderBuffer.Bind();
    renderBuffer.UseDepthAndStencil(width, height);

    renderBuffer.Unbind();

    glad_glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer.id);

    OutlineEffect.Unbind();


}



unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere() {
    if (sphereVAO == 0) {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = static_cast<unsigned int>(indices.size());

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        unsigned int stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}




void Renderer::Render(Scene& scene)
{
   
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(scene.MainCamera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    auto view = scene.MainCamera.GetViewMatrix();
    auto model = glm::mat4(1.0f);   

    // Skybox 
    glDepthMask(GL_FALSE);
    Shader shader = shaders.at(static_cast<int>(RenderPass::SKYBOX));

    shader.use();

    shader.setMat4("projection", projection);
    auto centeredView = glm::mat4(glm::mat3(scene.MainCamera.GetViewMatrix()));
    shader.setMat4("view", centeredView);

    glActiveTexture(GL_TEXTURE0);
    scene.skybox.Bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);

    scene.skybox.Unbind();

    glDepthMask(GL_TRUE);


    // Phong lighting
    shader = shaders.at(static_cast<int>(RenderPass::DEFAULT));

    shader.use();
    shader.setVec3("cameraPos", scene.MainCamera.Position);
    shader.setInt("skybox", 11);

    glActiveTexture(GL_TEXTURE11);
    scene.skybox.Bind();
    glActiveTexture(GL_TEXTURE0);

    model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    for (auto entity : scene.entities) {
        entity.Draw(shader);
    }
    scene.skybox.Unbind();
    

    // PBR 
    shader = shaders.at(static_cast<int>(RenderPass::PBR));

    shader.use();
    shader.setInt("albedoMap", 0);
    shader.setInt("normalMap", 1);
    shader.setInt("metallicMap", 2);
    shader.setInt("roughnessMap", 3);
    shader.setInt("aoMap", 4);

    shader.setMat4("projection", projection);
    view = scene.MainCamera.GetViewMatrix();
    shader.setMat4("view", view);
    shader.setVec3("camPos", scene.MainCamera.Position);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedo);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, metallic);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, roughness);


    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, ao);


    model = glm::mat4(1.0f);
    for (int row = 0; row < nrRows; ++row) {
        for (int col = 0; col < nrColumns; ++col) {

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(
                (col - (nrColumns / 2)) * spacing,
                (row - (nrRows / 2)) * spacing,
                0.0f
            ));

            shader.setMat4("model", model);
            shader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
            renderSphere();

        }
    }

    // Render light source
    for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i) {
        glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        newPos = lightPositions[i];
        shader.setVec3("lightPositions[" + std::to_string(i)+"]", newPos);
        shader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

        model = glm::mat4(1.0f);
        model = glm::translate(model, newPos);
        model = glm::scale(model, glm::vec3(0.5f));
        shader.setMat4("model", model);
        shader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));

        renderSphere();
    }





    /*
    Shader OutlineShader;
    OutlineShader.Load("../Shaders/shader.vs", "../Shaders/outlineshader.fs");

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    //OutlineEffect.Bind();
    //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    OutlineShader.use();
    OutlineShader.setMat4("model", glm::scale(glm::mat4(1.0f), glm::vec3(1.05f, 1.05f, 1.05f)));
    OutlineShader.setMat4("view", view);
    OutlineShader.setMat4("projection", projection);
    OutlineShader.setVec3("outlineColor", glm::vec3(0.28f, 0.10f, 0.26f));

    for (auto entity : scene.entities) {
        entity.Draw(OutlineShader);
    }

            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);
                
        */

     
    
   

   

    
    // 4. draw result to screen
    /*
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glDisable(GL_DEPTH_TEST);

    Shader shader;
    shader.Load("../Shaders/Framebuffers.vs", "../Shaders/Framebuffers.fs");
    shader.use();
    shader.setInt("screenTexture",0);
    glBindVertexArray(ScreenVAO);

    glBindTexture(GL_TEXTURE_2D, ColourBuffer->id);

    glDrawArrays(GL_TRIANGLES, 0, 6);


    // Reset stencil 
    glEnable(GL_DEPTH_TEST);
    */
}

void Renderer::Shutdown() {


    framebuffer.Unbind();
}
