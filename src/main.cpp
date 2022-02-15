#include <stdio.h>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "stb_image.h"
#include "shader.h"
#include "camera.h"

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));
float lastX = 400, lastY = 300;
bool firstMouse  = true;

glm::vec3 lightpos(1.2f, 1.0f, 2.0f);

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


   Shader lightshader("lightsource.vs", "lightsource.fs");
   Shader shader ("shader.vs", "shader.fs");

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

   unsigned int VBO, VAO;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);

   glBindVertexArray(VAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3*sizeof(float)));
   glEnableVertexAttribArray(1);

   unsigned int lightVAO;
   glGenVertexArrays(1, &lightVAO);
   glBindVertexArray(lightVAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);

   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 6* sizeof(float), (void*) 0);
   glEnableVertexAttribArray(0);


glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
glfwSetCursorPosCallback(window, mouse_callback); 
glfwSetScrollCallback(window, scroll_callback);

glEnable(GL_DEPTH_TEST);
while(!glfwWindowShouldClose(window))
{
   float currentFrame = glfwGetTime();
   deltaTime = currentFrame - lastFrame;
   lastFrame = currentFrame; 

   processInput(window);
   
   glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  

   glm::mat4 model         = glm::mat4(1.0f); 
   glm::mat4 view          = glm::mat4(1.0f);
   glm::mat4 projection    = glm::mat4(1.0f);
   
   view = camera.GetViewMatrix();
   
   projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
   
   float orbital_speed = .4 ; 

   // move light around 
   lightpos.x = 1.0 + sin(glfwGetTime()) * 2.0f;
   lightpos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

   
   // draw cubes
   shader.use();
   
   shader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
   shader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
   shader.setVec3("material.specular", glm::vec3(0.5, 0.5f, 0.5f));
   shader.setFloat("material.shininess", 32.0f);


   glm::vec3 lightColor;
   lightColor.x = sin(glfwGetTime() * 2.0f);
   lightColor.y = sin(glfwGetTime() * 0.7f);
   lightColor.z = sin(glfwGetTime() * 1.3f);

   glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
   glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);



   shader.setVec3("light.position", lightpos);

   shader.setVec3("light.ambient", ambientColor);
   shader.setVec3("light.diffuse", diffuseColor);
   shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
   


   shader.setVec3("viewPos", camera.Position);

   shader.setMat4("view", view);
   shader.setMat4("projection", projection);

   
   model = glm::mat4(1.0);
   shader.setMat4("model", model);

   glBindVertexArray(VAO);
   glDrawArrays(GL_TRIANGLES, 0, 36);




   // draw lights
   lightshader.use();
   lightshader.setMat4("view", view);
   lightshader.setMat4("projection", projection);
   
   model = glm::mat4(1.0f);      
   model = glm::translate(model, lightpos);
   model = glm::scale(model, glm::vec3(0.2f));

   lightshader.setMat4("model", model);

   glBindVertexArray(lightVAO);
   glDrawArrays(GL_TRIANGLES, 0, 36);

   
   glfwSwapBuffers(window);
   glfwPollEvents();
}

glDeleteVertexArrays(1, &VAO);
glDeleteBuffers(1, &VBO);

glfwTerminate();

return 0;


}