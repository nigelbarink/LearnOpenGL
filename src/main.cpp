#include <stdio.h>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
   glViewport(0,0, width, height);
}

void processInput( GLFWwindow* window){
   if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

}

int main() {
   printf("Hello OpenGL!");
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


   GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
   if( window == NULL){
      printf("Failed to create GLFW window!");
      glfwTerminate();
      return -1; 
   }
   glfwMakeContextCurrent(window);

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      printf("Failed to initialize GLAD!");
      return -1;
   }

   glViewport(0,0, 800, 600);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


   glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

   Shader shader("shader.vs", "shader.fs");




   float vertices[] = {
      0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom left
      0.0f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f  // top

   };

   unsigned int indices[] = {
         0, 1,2 // first triangle
   };

   // Vertex buffer objects!!
   unsigned int VBO, VAO, EBO;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   glBindVertexArray(VAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // Position attribute
   glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   // Color attribute
   glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
   glEnableVertexAttribArray(1);

   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindVertexArray(0);

   // WireFrame mode
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

   while(!glfwWindowShouldClose(window))
   {
      processInput(window);

      glClear(GL_COLOR_BUFFER_BIT);


      shader.use();
      shader.setFloat("HorizontalOffset", .3);

      glBindVertexArray(VAO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      
      glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT,  0);
      

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);

   glfwTerminate();

   return 0;


}