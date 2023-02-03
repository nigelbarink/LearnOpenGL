#include "OutlinePass.h"
#include "Renderer.h"

OutlinePass::OutlinePass(Model& sceneObject, glm::mat4& model, glm::mat4& projection, Camera& camera) 
: RenderPass("Renderpass - Outline", "../Shaders/shader.vs", "../Shaders/outlineshader.fs"  ) , m_model(sceneObject), camera(camera), projection(projection), model(model)
{
    // Framebuffer outline effect
    std::cout << "Create Framebuffer" << std::endl;
    
      m_outlineFrameBuffer = FrameBuffer();
      m_outlineFrameBuffer.Bind();

      // Create a texture attachment (colour attachment)
      std::cout << "Create Texture" << std::endl;
      m_depthTexture = CreateTexture(800,600);
   
      // attach texture to the frame buffer as a colour attachment
      m_outlineFrameBuffer.Attach(*m_depthTexture);

      // Add depth buffer attachment 
      std::cout << "Create RenderBuffer" << std::endl;
      m_outlineRenderBuffer = RenderBuffer();
      m_outlineRenderBuffer.Bind();
      m_outlineRenderBuffer.UseDepthAndStencil();
      m_outlineRenderBuffer.Unbind();

      // attach depth buffer to our framebuffer
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_outlineRenderBuffer.id);
      m_outlineFrameBuffer.Unbind();

      std::cout << "Success!" << std::endl;

}

OutlinePass::~OutlinePass(){

    delete m_depthTexture;
}

void OutlinePass::Render()
{
    std::cout << "Start outline render" << std::endl;
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    m_shader.use();
    m_shader.setMat4("model", model);
    m_shader.setMat4("view", camera.GetViewMatrix());
    m_shader.setMat4("projection", projection);
    m_shader.setVec3("outlineColor", glm::vec3(0.28, 0.10, 0.26)); 

    model = glm::scale(model, glm::vec3(1.05f,1.05f, 1.05f));
    m_shader.setMat4("model", model);
    
    std::cout << "Drawing model" << std::endl;
    m_model.Draw(m_shader);


    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilMask(0xFF);

    std::cout << "Finished outline render" << std::endl;

}