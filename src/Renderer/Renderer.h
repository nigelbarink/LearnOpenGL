#pragma once

#include "../model.h"
#include "../Primitives/camera.h"
#include "../Primitives/shader.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "../Primitives/Scene.h"
#include <map>

class Renderer
{
   public:
       Renderer() = default ;
      void Setup();
      void resize(int width, int height);
      void Render(Scene& scene);
      void Shutdown();


   private:
      GLuint ScreenVAO, VBO;
      FrameBuffer framebuffer;
      FrameBuffer OutlineEffect;
      RenderBuffer renderbufferObject;
      Texture* ColourBuffer;
      
      glm::vec3 lightpos = glm::vec3(-0.2f, -1.0f, -0.3f);

      std::map<int, Shader> shaders;


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
