#include "ScenePass.h"

void ScenePass::Render()
{
    m_shader.use();
    m_shader.setVec3("cameraPos", camera.Position);

    if (m_skybox)
    {
        glActiveTexture(GL_TEXTURE11);
        m_skybox->Bind();
        glActiveTexture(GL_TEXTURE0);

        m_shader.setInt("skybox",11);
    }
    
    m_shader.setMat4("model", model);
    m_shader.setMat4("view", camera.GetViewMatrix());
    m_shader.setMat4("projection", projection);
 

    m_scene.Draw(m_shader);
}

void ScenePass::Setup()
{
}

void ScenePass::SetSkybox(Skybox& skybox)
{
    m_skybox = &skybox;
}