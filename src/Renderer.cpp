#include "Renderer.h"
#include <iostream>

void Renderer::Setup()
{
    framebuffer = new FrameBuffer();
    framebuffer->Bind();

    ColourBuffer = CreateTexture(800,600);
    framebuffer->Attach(*ColourBuffer);

    renderbufferObject = new RenderBuffer();
    renderbufferObject->Bind();
    renderbufferObject->UseDepthAndStencil();
    renderbufferObject->Unbind();

    framebuffer->Attach(*renderbufferObject);

    if(framebuffer->IsComplete() == false ){
    std::cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete! " << std::endl;
    }  

    framebuffer->Unbind();
}

void Renderer::Render()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame; 

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Recalculate the Camera's view matrix 
    view = camera.GetViewMatrix();
    // Reset the model matrix to an identity matrix
    model = glm::mat4(1.0f);


    framebuffer->Bind();
    std::cout << "Starting Render passes :" << std::endl;

    // Outline of render steps

    // 1. Skybox pass
    // 2. Scene pass
    // 3. outline pass
    // SKIP for now!

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 4. draw result to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    m_shader->use();
    m_shader->setInt("screenTexture",0);
    glBindVertexArray(ScreenVAO);
    glBindTexture(GL_TEXTURE_2D, ColourBuffer->id);
    glDrawArrays(GL_TRIANGLES, 0, 6);


    // Reset stencil 
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);   
    glEnable(GL_DEPTH_TEST); 

}



Renderer::Renderer(Camera& camera) 
: camera(camera)
{
    deltaTime = 0.0f; 
    /// Create and compile the screen shader
    m_shader = new Shader ("Shaders/Framebuffers.vs", "Shaders/Framebuffers.fs" );

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
    delete framebuffer;
    delete renderbufferObject;
    delete m_shader;
    delete ColourBuffer;
}