#include <stdio.h>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));
float lastX = 400, lastY = 300;
bool firstMouse  = true;

glm::vec3 lightpos(-0.2f, -1.0f, -0.3f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
   glViewport(0,0, width, height);
}

void processInput( GLFWwindow* window){
   if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

   const float CameraSpeed = 2.5f * deltaTime; 
   if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.ProcessKeyboard(FORWARD, deltaTime);
   if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.ProcessKeyboard(BACKWARD, deltaTime);
   if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.ProcessKeyboard(LEFT, deltaTime);
   if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.ProcessKeyboard(RIGHT,deltaTime);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos){
   if (firstMouse) // initially set to true
   {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
   }
   float xoffset = xpos - lastX;
   float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
   
   lastX = xpos;
   lastY = ypos;

   camera.ProcessMouseMovement(xoffset,yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
   camera.ProcessMouseScroll(yoffset);
}

int main() {
   printf("Hello OpenGL!\n");
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


   GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
   if( window == NULL){
      printf("Failed to create GLFW window!\n");
      glfwTerminate();
      return -1; 
   }
   glfwMakeContextCurrent(window);

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      printf("Failed to initialize GLAD!\n");
      return -1;
   }

   glViewport(0,0, 800, 600);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
   glfwSetCursorPosCallback(window, mouse_callback); 
   glfwSetScrollCallback(window, scroll_callback);

   glEnable(GL_DEPTH_TEST);
   stbi_set_flip_vertically_on_load(true);


   Shader shader ("shader.vs", "shader.fs");

   Model backpack("Models/backpack.obj");



glEnable(GL_DEPTH_TEST);
while(!glfwWindowShouldClose(window))
{
   float currentFrame = glfwGetTime();
   deltaTime = currentFrame - lastFrame;
   lastFrame = currentFrame; 

   processInput(window);
   
   glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   shader.use();

   glm::mat4 model         = glm::mat4(1.0f); 
   glm::mat4 view          = glm::mat4(1.0f);
   glm::mat4 projection    = glm::mat4(1.0f);
   
   view = camera.GetViewMatrix();
   projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);    
   model = glm::mat4(1.0);

   shader.setMat4("projection", projection);
   shader.setMat4("view", view);
   shader.setMat4("model", model);

   backpack.Draw(shader);
   

   
   glfwSwapBuffers(window);
   glfwPollEvents();
}


glfwTerminate();

return 0;


}