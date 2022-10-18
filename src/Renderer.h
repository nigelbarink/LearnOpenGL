#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cmath>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shader.h"
#include "model.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "Cubemap.h"
#include "SkyboxPass.h"
#include "ScenePass.h"

class Renderer
{
   public:
      void Render();
      void Setup();

      Renderer(Camera& camera);
      ~Renderer();

   private:
      Camera& camera;
      float deltaTime;	// Time between current frame and last frame
      float lastFrame = 0.0f; // Time of last frame
      GLuint ScreenVAO, VBO;
      FrameBuffer* framebuffer;
      RenderBuffer* renderbufferObject;
      Texture* ColourBuffer;

      Shader* m_shader;

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
