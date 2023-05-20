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
static enum class RenderPass {
    NONE = 0,
    SKYBOX,
    DEFAULT,
};
const int num_passes = static_cast<int>(RenderPass::DEFAULT) ;

Texture* colourAttachment;


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

void Renderer::Render(Scene& scene)
{
    static RenderPass currentPass = RenderPass::SKYBOX;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(scene.MainCamera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    auto view = scene.MainCamera.GetViewMatrix();
    auto model = glm::mat4(1.0f);   

    std::cout << "SKYBOX PASS!" << std::endl;
    glDepthMask(GL_FALSE);
    Shader shader = shaders.at(static_cast<int>(RenderPass::SKYBOX));

    shader.use();

    shader.setMat4("projection", projection);
    auto centeredView = glm::mat4(glm::mat3(scene.MainCamera.GetViewMatrix()));
    shader.setMat4("view", centeredView);

    scene.skybox.Bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);

    scene.skybox.Unbind();

    glDepthMask(GL_TRUE);


    std::cout << "DEFAULT PASS!" << std::endl;
    shader = shaders.at(static_cast<int>(RenderPass::DEFAULT));

    shader.use();
    shader.setVec3("cameraPos", scene.MainCamera.Position);
    shader.setInt("skybox", 11);

    glActiveTexture(GL_TEXTURE11);
    scene.skybox.Bind();
    glActiveTexture(GL_TEXTURE0);


    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    for (auto entity : scene.entities) {
        entity.Draw(shader);
    }
    scene.skybox.Unbind();

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
