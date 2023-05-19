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
void Renderer::Setup()
{
    shader.Load("../Shaders/Framebuffers.vs", "../Shaders/Framebuffers.fs");

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

}


void Renderer::Render(Scene& scene)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   
    glm::mat4 projection = glm::perspective(glm::radians(scene.cameras.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    auto view = scene.cameras.GetViewMatrix();
    auto model = glm::mat4(1.0f);


    // 1. Skybox pass
    /*
    Shader cubemap;
    cubemap.Load("../Shaders/skybox.vs", "../Shaders/Cubemap.fs");

    glDepthMask(GL_FALSE);

    cubemap.use();

    cubemap.setMat4("projection", projection);
    auto centeredView = glm::mat4(glm::mat3(scene.cameras.GetViewMatrix()));
    cubemap.setMat4("view", centeredView);

    skybox.Bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);

    skybox.Unbind();

    glDepthMask(GL_TRUE);
    */


    // 2. Scene pass
    Shader  blinnPhong;
    blinnPhong.Load("../Shaders/shader.vs", "../Shaders/shader.fs");

    blinnPhong.use();
    blinnPhong.setVec3("cameraPos", scene.cameras.Position);

    /*
    glActiveTexture(GL_TEXTURE11);
    m_skybox.Bind(); // Segmentation Fault HERE!!
    glActiveTexture(GL_TEXTURE0);

    */

    blinnPhong.setInt("skybox", 11);

    blinnPhong.setMat4("model", model);
    blinnPhong.setMat4("view", view);
    blinnPhong.setMat4("projection", projection);

    for (auto entity : scene.entities) {
        entity.Draw(blinnPhong);
    }

    // m_skybox.Unbind();

    // 3. outline pass
    //Shader OutlineShader;
    //OutlineShader.Load("../Shaders/shader.vs", "../Shaders/outlineshader.fs");


    /*
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    */

    // 4. draw result to screen
    /*glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    shader.use();
    shader.setInt("screenTexture",0);
    glBindVertexArray(ScreenVAO);

    glBindTexture(GL_TEXTURE_2D, ColourBuffer->id);

    glDrawArrays(GL_TRIANGLES, 0, 6);


    // Reset stencil 
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);   
    glEnable(GL_DEPTH_TEST); 
    */
}

void Renderer::Shutdown() {

    framebuffer.Unbind();
}

Renderer::Renderer() {
  


    // auto outlinepass = OutlinePass(scene , model, projection, camera); 
   

}

Renderer::~Renderer()
{

}