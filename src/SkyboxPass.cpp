#include "SkyboxPass.h"


SkyboxPass::~SkyboxPass()
{
}

void SkyboxPass::Setup()
{
}

void SkyboxPass::Render()
{
    glDepthMask(GL_FALSE);

   m_shader.use();
   m_shader.setMat4("projection", projection);

   glm::mat4 centeredView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
   m_shader.setMat4("view", centeredView);

   glBindVertexArray(skybox.m_skyboxVAO);
   skybox.Bind();
   glDrawArrays(GL_TRIANGLES, 0, 36);
   glDepthMask(GL_TRUE);
}