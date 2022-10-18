#include "OutlinePass.h"
#include "Renderer.h"
OutlinePass::OutlinePass(Shader& shader, Model& sceneObject, glm::mat4& model, glm::mat4& projection, Camera& camera) 
: RenderPass("Renderpass - Outline", shader ) , m_model(sceneObject), model(model), projection(projection), camera(camera)
{
}

OutlinePass::~OutlinePass(){

    delete m_depthTexture;
}

void OutlinePass::Setup()
{
          // Framebuffer outline effect
      m_outlineFrameBuffer = FrameBuffer();
      m_outlineFrameBuffer.Bind();

      // Create a texture attachment (colour attachment)
      m_depthTexture = CreateTexture(800,600);
   
      // attach texture to the frame buffer as a colour attachment
      m_outlineFrameBuffer.Attach(*m_depthTexture);

      // Add depth buffer attachment 
      m_outlineRenderBuffer = RenderBuffer();
      m_outlineRenderBuffer.Bind();

      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
      m_outlineRenderBuffer.Unbind();

      // attach depth buffer to our framebuffer
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_outlineRenderBuffer.id);
      m_outlineFrameBuffer.Unbind();

}

void OutlinePass::Render()
{
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

    m_model.Draw(m_shader);

    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilMask(0xFF);

}