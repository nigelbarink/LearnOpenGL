#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Window.h"
#include "Renderer.h"
#include "Skybox.h"
#include "shader.h"
#include "camera.h"
#include "Application.h"

class LearnOpenGL : Application 
{
   
public: 
   void Run () override{
      // Create a window
      Window window(800,600, "LearnOpenGL");

      Camera camera(glm::vec3(0.0f,0.0f,8.0f));
      
      Renderer renderer = Renderer(camera);
    
      renderer.Setup();
      
      stbi_set_flip_vertically_on_load(true);

      // Load scene resources 
      Shader shader ("Shaders/shader.vs", "Shaders/shader.fs");
      Shader outlineShader("Shaders/shader.vs","Shaders/outlineshader.fs");
      Shader skyboxShader("Shaders/skybox.vs", "Shaders/Cubemap.fs");

      Model backpack("Models/backpack.obj");
      int bullshit = 0;
      std::vector<std::string> faces = {
         "Textures/skybox/right.jpg",
         "Textures/skybox/left.jpg",
         "Textures/skybox/top.jpg",
         "Textures/skybox/bottom.jpg",
         "Textures/skybox/front.jpg",
         "Textures/skybox/back.jpg"
      };

      Skybox skybox= Skybox ();
      skybox.Bind();
      skybox.loadCubeTextures(faces);
      skybox.BuildVAO();

      //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      while(!window.shouldClose())
      {
         //window.GetInput();

         renderer.Render();
 
         window.SwapBuffers();
         glfwPollEvents();
      }

   }
};

/*

void processInput( GLFWwindow* window){
   if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

   const float CameraSpeed = 2.5f * Renderer::deltaTime; 
   if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      Renderer::camera.ProcessKeyboard(FORWARD, Renderer::deltaTime);
   if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      Renderer::camera.ProcessKeyboard(BACKWARD, Renderer::deltaTime);
   if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
       Renderer::camera.ProcessKeyboard(LEFT, Renderer::deltaTime);
   if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      Renderer::camera.ProcessKeyboard(RIGHT,Renderer::deltaTime);
}

*/


int main() 
{
   printf("Hello OpenGL!\n");
   LearnOpenGL().Run();
   return 0;
}
