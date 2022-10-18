#include "Skybox.h"

void Skybox::BuildVAO(){
    // Create SkyboxVAO
    
      glGenVertexArrays(1, &m_skyboxVAO);
      glBindVertexArray(m_skyboxVAO);
      
      glGenBuffers(1, &m_skyboxVBO);
      glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);

      glBufferData(GL_ARRAY_BUFFER, m_skyboxVertices.size() * sizeof(float), &m_skyboxVertices[0], GL_STATIC_DRAW);

      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE, 3 * sizeof(float),(void*) 0);

      glBindVertexArray(0);
}

Skybox::Skybox() : Cubemap()
{

}

Skybox::~Skybox(){}
