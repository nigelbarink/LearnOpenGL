#pragma once
#include <vector>
#include "Cubemap.h"


class Skybox : public Cubemap {
public:
    Skybox ();
    ~Skybox ();

    void BuildVAO();
    GLuint m_skyboxVAO;
private:
      GLuint  m_skyboxVBO;
      std::vector<float> m_skyboxVertices = {
         // positions          
         -1.0f,  1.0f, -1.0f,
         -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,

         -1.0f, -1.0f,  1.0f,
         -1.0f, -1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f,  1.0f,
         -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

         -1.0f, -1.0f,  1.0f,
         -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         -1.0f, -1.0f,  1.0f,

         -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f, -1.0f,

         -1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
      };
};
