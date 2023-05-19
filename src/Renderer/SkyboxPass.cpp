#include "SkyboxPass.h"
SkyboxPass::SkyboxPass(Skybox& skybox, Camera& camera, glm::mat4& projection) : RenderPass("Render Pass - Skybox", "../Shaders/skybox.vs", "../Shaders/Cubemap.fs"),
skybox(skybox), 
camera(camera),
projection(projection)
{

}



void SkyboxPass::Render()
{
    glDepthMask(GL_FALSE);

   m_shader.use();

   m_shader.setMat4("projection", projection);
   glm::mat4 centeredView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
   m_shader.setMat4("view", centeredView);



   skybox.Bind();
   

   glDrawArrays(GL_TRIANGLES, 0, 36);
   
   skybox.Unbind();
   
   glDepthMask(GL_TRUE);
}