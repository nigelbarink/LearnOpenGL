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
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
   glfwSetCursorPosCallback(window, mouse_callback); 
   glfwSetScrollCallback(window, scroll_callback);

   glEnable(GL_DEPTH_TEST);
   stbi_set_flip_vertically_on_load(true);


   Shader shader ("shader.vs", "shader.fs");
   Shader outlineShader("shader.vs","outlineshader.fs");

   Model backpack("Models/backpack.obj");


glEnable(GL_DEPTH_TEST);
glEnable(GL_STENCIL_TEST);
glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

///glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
while(!glfwWindowShouldClose(window))
{
   float currentFrame = glfwGetTime();
   deltaTime = currentFrame - lastFrame;
   lastFrame = currentFrame; 
  // std::cout << "Delta Time: " << deltaTime << std::endl;
   processInput(window);
   
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


   glm::mat4 model         = glm::mat4(1.0f); 
   
   glm::mat4 view = camera.GetViewMatrix();
   glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);   
   model = glm::mat4(1.0);


   glStencilFunc(GL_ALWAYS, 1, 0xff);
   glStencilMask(0xFF);

   shader.use();
   shader.setMat4("model", model);
   shader.setMat4("view", view);
   shader.setMat4("projection", projection);


   backpack.Draw(shader);


   glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
   glStencilMask(0x00);
   glDisable(GL_DEPTH_TEST);

   outlineShader.use();
   outlineShader.setMat4("model", model);
   outlineShader.setMat4("view", view);
   outlineShader.setMat4("projection", projection);
   outlineShader.setVec3("outlineColor", glm::vec3(0.28, 0.10, 0.26)); 

   model = glm::scale(model, glm::vec3(1.05f,1.05f, 1.05f));
  // model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
   outlineShader.setMat4("model", model);


   backpack.Draw(outlineShader);
   
   glStencilMask(0xFF);
   glStencilFunc(GL_ALWAYS, 1, 0xFF);   
   glEnable(GL_DEPTH_TEST); 


   glfwSwapBuffers(window);
   glfwPollEvents();
}


glfwTerminate();

return 0;


}