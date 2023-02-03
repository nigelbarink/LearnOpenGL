#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Window.h"
#include "Renderer/Renderer.h"
#include "Primitives/Skybox.h"
#include "Primitives/shader.h"
#include "Primitives/camera.h"
#include "Application.h"
#include <stb_image.h>

void processInput( GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

float deltaTime = 0.0f; ;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
bool firstMouse  = true;
float lastx = 400, lasty = 300;

Camera camera(glm::vec3(0.0f,0.0f,8.0f));

class LearnOpenGL : Application 
{
public: 
   void Run () override
   {
      // Create a window
      Window window(800,600, "LearnOpenGL");
      
      stbi_set_flip_vertically_on_load(true);

      Model backpack("../Models/backpack.obj");

      Renderer renderer = Renderer(camera, backpack);
      renderer.Setup();

      //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      while(!window.shouldClose())
      {
         float currentFrame = glfwGetTime();
         deltaTime = currentFrame - lastFrame;
         lastFrame = currentFrame; 

         glfwPollEvents();
         processInput(window.ptr());

         renderer.Render();
 
         window.SwapBuffers();
      }

   }
};

void processInput( GLFWwindow* window)
{
   if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

   const float CameraSpeed = 0.5f * deltaTime; 
   if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.ProcessKeyboard(FORWARD, CameraSpeed);
   if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.ProcessKeyboard(BACKWARD,CameraSpeed);
   if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.ProcessKeyboard(LEFT, CameraSpeed);
   if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.ProcessKeyboard(RIGHT,CameraSpeed);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse){
        lastx = xpos;
        lasty = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastx;
    float yoffset = ypos - lasty;

    lastx = xpos;
    lasty - ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0, width,height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    camera.ProcessMouseScroll(yoffset);
}

int main() 
{
   printf("Hello OpenGL!\n");
   LearnOpenGL().Run();
   return 0;
}
