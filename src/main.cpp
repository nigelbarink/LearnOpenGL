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


   Shader lightshader("lightsource.vs", "lightsource.fs");
   Shader shader ("shader.vs", "shader.fs");

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

   unsigned int VBO, VAO;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);

   glBindVertexArray(VAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3*sizeof(float)));
   glEnableVertexAttribArray(1);

   glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE, 8 * sizeof(float), (void*) (6*sizeof(float)));
   glEnableVertexAttribArray(2);


   unsigned int lightVAO;
   glGenVertexArrays(1, &lightVAO);
   glBindVertexArray(lightVAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);

   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 8* sizeof(float), (void*) 0);
   glEnableVertexAttribArray(0);

// Load diffuse texture
int width, height, nrChannels;
unsigned int diffuseMap;
glGenTextures(1, &diffuseMap);

glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, diffuseMap);

glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

unsigned char* data = stbi_load("Textures/container2.png", &width, &height, &nrChannels, 0);
if(data){
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
   glGenerateMipmap(GL_TEXTURE_2D);
} else{
   std::cout << "Error loading texture...." << std::endl;
}

stbi_image_free(data);

// Load Specular texture
unsigned int specularMap;
glGenTextures(1, &specularMap);

glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, specularMap);

unsigned char* specular_data = stbi_load("Textures/container2_specular.png", &width, &height, &nrChannels, 0);
if( specular_data ){
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, specular_data);
      glGenerateMipmap(GL_TEXTURE_2D);
}else{
   std::cout << "Error loading texture...." << std::endl;
}

stbi_image_free(specular_data);



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



   
   // draw cubes
   shader.use();
   shader.setInt("material.diffuse", 0);
   shader.setInt("material.specular", 1);
   shader.setVec3("material.specular", glm::vec3(0.5, 0.5f, 0.5f));
   shader.setFloat("material.shininess", 32.0f);



   shader.setVec3("light.position", camera.Position);
   shader.setVec3("light.direction", camera.Front);
   shader.setFloat("light.cutoff", glm::cos(glm::radians(12.5f)));
   shader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

   shader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
   shader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
   shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
   


   shader.setFloat("light.constant", 1.0f);
   shader.setFloat("light.linear", 0.09f);
   shader.setFloat("light.quadratic", 0.032f);

   shader.setVec3("viewPos", camera.Position);

   shader.setMat4("view", view);
   shader.setMat4("projection", projection);

   
   model = glm::mat4(1.0);
   shader.setMat4("model", model);

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, diffuseMap);
   
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, specularMap);

   for( unsigned int i = 0; i < 10; i++ ){
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model , cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.setMat4("model", model);

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);


   }




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