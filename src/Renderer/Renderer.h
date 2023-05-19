#pragma once

#include "../model.h"
#include "../Primitives/camera.h"
#include "../Primitives/shader.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "../Primitives/Scene.h"

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
      RenderBuffer renderbufferObject;
      Texture* ColourBuffer;
      Shader shader;

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
