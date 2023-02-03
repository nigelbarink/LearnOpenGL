#pragma once

#include "../model.h"
#include "../Primitives/camera.h"
#include "../Primitives/shader.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "ScenePass.h"
#include "OutlinePass.h"
#include "SkyboxPass.h"


class Renderer
{
   public:
      void Render();
      void Setup();

      Renderer(Camera& camera,Model& scene);
      ~Renderer();
      
     
   private:
      ScenePass scenepass;
      SkyboxPass skyboxpass;
      OutlinePass outlinepass;

      Camera& camera;
      GLuint ScreenVAO, VBO;
      FrameBuffer framebuffer;
      RenderBuffer renderbufferObject;
      Texture* ColourBuffer;
      Skybox skybox;

      Shader m_shader;

      glm::mat4 model = glm::mat4(1.0f); 
      glm::mat4 view;
      glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);   
      
      glm::vec3 lightpos = glm::vec3(-0.2f, -1.0f, -0.3f);

      std::vector<float> ScreenVertices = {
         // vertex ,    uv 
         -1.0f, 1.0f,   0.0f, 1.0f,
         -1.0f,-1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 0.0f,

         -1.0f, 1.0f,   0.0f, 1.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
         1.0f, 1.0f,    1.0f, 1.0f,
      };

};
