#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Window.h"
#include "Renderer/Renderer.h"
#include "Primitives/Skybox.h"
#include "Primitives/shader.h"
#include "Primitives/camera.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Application.h"
#include "Primitives/Scene.h"


void processInput( GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
const int WIDTH = 800, HEIGHT = 600;
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
      Window window(WIDTH,HEIGHT, "LearnOpenGL");
      
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO();(void)io;
      

      ImGui::StyleColorsDark();

      ImGui_ImplGlfw_InitForOpenGL(window.ptr(), true);
      ImGui_ImplOpenGL3_Init("#version 460");

      stbi_set_flip_vertically_on_load(true);
      Model backpack("../Models/backpack.obj");

      Scene scene;
      scene.entities.push_back(backpack);

      scene.cameras = camera;


      std::vector<std::string> faces = {
          "../Textures/skybox/right.jpg",
          "../Textures/skybox/left.jpg",
          "../Textures/skybox/top.jpg",
          "../Textures/skybox/bottom.jpg",
          "../Textures/skybox/front.jpg",
          "../Textures/skybox/back.jpg"
      };
      Skybox skybox = Skybox();
      skybox.loadCubeTextures(faces);

      Renderer renderer = Renderer();
      renderer.resize(WIDTH, HEIGHT);
      renderer.Setup();

     

      //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      while(!window.shouldClose())
      {
         float currentFrame = glfwGetTime();
         deltaTime = currentFrame - lastFrame;
         lastFrame = currentFrame; 

         glfwPollEvents();
         processInput(window.ptr());
         renderer.Render(scene);


         ImGui_ImplOpenGL3_NewFrame();
         ImGui_ImplGlfw_NewFrame();
         ImGui::NewFrame();
         ImGui::ShowDemoWindow(nullptr);

         ImGui::Render();

         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
 
         window.SwapBuffers();
      }

      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();

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
