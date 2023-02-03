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

void Renderer::Setup()
{
    framebuffer.Bind();

    ColourBuffer = CreateTexture(800,600);
    framebuffer.Attach(*ColourBuffer);

    renderbufferObject.Bind();
    renderbufferObject.UseDepthAndStencil();
    renderbufferObject.Unbind();

    framebuffer.Attach(renderbufferObject);

    if(framebuffer.IsComplete() == false ){
    std::cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete! " << std::endl;
    }  

    framebuffer.Unbind();
}

void Renderer::Render( )
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Recalculate the Camera's view matrix 
    view = camera.GetViewMatrix();
    // Reset the model matrix to an identity matrix
    model = glm::mat4(1.0f);

    framebuffer.Bind();

    // 1. Skybox pass
    skyboxpass.Render();

    // 2. Scene pass
    scenepass.Render();

    // 3. outline pass
    
    outlinepass.Render();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    // 4. draw result to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    m_shader.use();
    m_shader.setInt("screenTexture",0);
    glBindVertexArray(ScreenVAO);
    
    glBindTexture(GL_TEXTURE_2D, ColourBuffer->id);
    glDrawArrays(GL_TRIANGLES, 0, 6);


    // Reset stencil 
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);   
    glEnable(GL_DEPTH_TEST); 

}

Renderer::Renderer(Camera& camera, Model& scene) :
 camera(camera),
 renderbufferObject(),
 framebuffer(),
 skybox(),
 skyboxpass(skybox, camera, projection),
 scenepass(skybox, scene,camera,projection, model),
 outlinepass(scene, model, projection, camera),
 m_shader("../Shaders/Framebuffers.vs", "../Shaders/Framebuffers.fs")
{

    std::vector<std::string> faces = {
        "../Textures/skybox/right.jpg",
        "../Textures/skybox/left.jpg",
        "../Textures/skybox/top.jpg",
        "../Textures/skybox/bottom.jpg",
        "../Textures/skybox/front.jpg",
        "../Textures/skybox/back.jpg"
    };
    skybox.loadCubeTextures(faces);


    // auto outlinepass = OutlinePass(scene , model, projection, camera); 
    // Create ScreenVAO
    glGenVertexArrays(1, &ScreenVAO);
    glBindVertexArray(ScreenVAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, ScreenVertices.size() * sizeof(float), &ScreenVertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2, GL_FLOAT,GL_FALSE, 4 * sizeof(float),(void*) 0 );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));

    glBindVertexArray(0);

    // Enable features
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_MULTISAMPLE);

}

Renderer::~Renderer()
{

}