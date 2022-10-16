#include <stdio.h>
#include <cmath>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "FrameBuffer.hpp"
#include "RenderBuffer.hpp"
#include "Cubemap.h"

// https://learnopengl.com/Advanced-OpenGL/Framebuffers

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));
float lastX = 400, lastY = 300;
bool firstMouse  = true;

glm::mat4 model = glm::mat4(1.0f); 
glm::mat4 view;
glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);   
  
glm::vec3 lightpos(-0.2f, -1.0f, -0.3f);

std::vector<float> ScreenVertices = {
   // vertex ,    uv 
   -1.0f, 1.0f,   0.0f, 1.0f,
   -1.0f,-1.0f,   0.0f, 0.0f,
   1.0f, -1.0f,   1.0f, 0.0f,

   -1.0f, 1.0f,   0.0f, 1.0f,
   1.0f, -1.0f,   1.0f, 0.0f,
   1.0f, 1.0f,    1.0f, 1.0f,
} ;

std::vector<std::string> faces = {
   "Textures/skybox/right.jpg",
   "Textures/skybox/left.jpg",
   "Textures/skybox/top.jpg",
   "Textures/skybox/bottom.jpg",
   "Textures/skybox/front.jpg",
   "Textures/skybox/back.jpg"
};

std::vector<float> skyboxVertices = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

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

GLFWwindow* CreateWindowWithOpenGLContext()
{
 glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


   GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
   if( window == NULL){
      printf("Failed to create GLFW window!\n");
      glfwTerminate();
      return nullptr; 
   }
   glfwMakeContextCurrent(window);

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      printf("Failed to initialize GLAD!\n");
      return nullptr;
   }

   glViewport(0,0, 800, 600);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
   glfwSetCursorPosCallback(window, mouse_callback); 
   glfwSetScrollCallback(window, scroll_callback);

   return window;

}

void Skybox_pass (GLuint& SkyboxVAO, Shader& skyboxShader, Cubemap& skycubemap)
{
   //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
   //glClear(GL_COLOR_BUFFER_BIT );

   glDepthMask(GL_FALSE);

   skyboxShader.use();
   skyboxShader.setMat4("projection", projection);

   glm::mat4 centeredView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
   skyboxShader.setMat4("view", centeredView);

   glBindVertexArray(SkyboxVAO);
   skycubemap.Bind();
   glDrawArrays(GL_TRIANGLES, 0, 36);
   glDepthMask(GL_TRUE);
}

void drawScene_pass(Shader& shader,  Model& Object, Cubemap& skybox)
{
   //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



   shader.use();

   shader.setVec3("cameraPos", camera.Position);
   skybox.Bind();
   shader.setMat4("model", model);
   shader.setMat4("view", view);
   shader.setMat4("projection", projection);

   Object.Draw(shader);
}

void outline_pass(  Shader& shader,  Model& Object)
{


   glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
   glStencilMask(0x00);
   glDisable(GL_DEPTH_TEST);

   shader.use();
   shader.setMat4("model", model);
   shader.setMat4("view", view);
   shader.setMat4("projection", projection);
   shader.setVec3("outlineColor", glm::vec3(0.28, 0.10, 0.26)); 

   model = glm::scale(model, glm::vec3(1.05f,1.05f, 1.05f));
   shader.setMat4("model", model);

   Object.Draw(shader);

   glStencilFunc(GL_ALWAYS, 1, 0xff);
   glStencilMask(0xFF);
}

void DrawToScreen( GLuint& VAO, GLuint& ScreenTexture, Shader& shader)
{
   // draw to screen
   glBindFramebuffer(GL_FRAMEBUFFER, 0);

   glDisable(GL_DEPTH_TEST);

   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);


   shader.use();
   shader.setInt("screenTexture",0);
   glBindVertexArray(VAO);
   glBindTexture(GL_TEXTURE_2D, ScreenTexture);
   glDrawArrays(GL_TRIANGLES, 0, 6);
}

int main() {
   printf("Hello OpenGL!\n");


   // Setup OpenGL Context
   GLFWwindow* window = CreateWindowWithOpenGLContext();

   if(window == nullptr)
      return -1;

   glEnable(GL_DEPTH_TEST);
   stbi_set_flip_vertically_on_load(true);

   // Load scene resources 
   Shader shader ("shader.vs", "shader.fs");
   Shader outlineShader("shader.vs","outlineshader.fs");
   Shader framebufferShader("Framebuffers.vs", "Framebuffers.fs" );
   Shader skyboxShader("skybox.vs", "Cubemap.fs");

   Model backpack("Models/backpack.obj");

   // Framebuffer outline effect
   FrameBuffer outlineFrameBuffer = FrameBuffer();
   outlineFrameBuffer.Bind();

   // Create a texture attachment (colour attachment)
   Texture* depthTexture = CreateTexture(800,600);
  
   // attach texture to the frame buffer as a colour attachment
   outlineFrameBuffer.Attach(*depthTexture);

   // Add depth buffer attachment 
   RenderBuffer outlineRenderBuffer = RenderBuffer();
   outlineRenderBuffer.Bind();

   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
   outlineRenderBuffer.Unbind();

   // attach depth buffer to our framebuffer
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, outlineRenderBuffer.id);
   outlineFrameBuffer.Unbind();

   // Post processing 
   FrameBuffer framebuffer = FrameBuffer();
   framebuffer.Bind();

   Texture* ColourBuffer = CreateTexture(800,600);
   framebuffer.Attach(*ColourBuffer);

   RenderBuffer renderbufferObject = RenderBuffer();
   renderbufferObject.Bind();
   renderbufferObject.UseDepthAndStencil();
   renderbufferObject.Unbind();

   framebuffer.Attach(renderbufferObject);

   if(framebuffer.IsComplete() == false ){
      std::cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete! " << std::endl;
   }  

   framebuffer.Unbind();

   Cubemap skybox= Cubemap ();
   skybox.Bind();
   skybox.loadCubeTextures(faces);

   // Create ScreenVAO
   #pragma region ScreenVAO
   GLuint ScreenVAO, VBO;
   glGenVertexArrays(1, &ScreenVAO);
   glBindVertexArray(ScreenVAO);
   
   glGenBuffers(1, &VBO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);

   glBufferData(GL_ARRAY_BUFFER, ScreenVertices.size() * sizeof(float), &ScreenVertices[0], GL_STATIC_DRAW);

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0,2, GL_FLOAT,GL_FALSE, 4 * sizeof(float),(void*) 0 );
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));


   glBindVertexArray(0);
#pragma endregion ScreenVAO


   // Create SkyboxVAO
   GLuint SkyboxVAO, SkyboxVBO;
   glGenVertexArrays(1, &SkyboxVAO);
   glBindVertexArray(SkyboxVAO);
   
   glGenBuffers(1, &SkyboxVBO);
   glBindBuffer(GL_ARRAY_BUFFER, SkyboxVBO);

   glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), &skyboxVertices[0], GL_STATIC_DRAW);

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE, 3 * sizeof(float),(void*) 0);

   glBindVertexArray(0);


   glEnable(GL_DEPTH_TEST);
   glEnable(GL_STENCIL_TEST);
   glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
while(!glfwWindowShouldClose(window))
{
   float currentFrame = glfwGetTime();
   deltaTime = currentFrame - lastFrame;
   lastFrame = currentFrame; 
   
   processInput(window);
   
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   // Recalculate the Camera's view matrix 
   view = camera.GetViewMatrix();
   // Reset the model matrix to an identity matrix
   model = glm::mat4(1.0f);


   framebuffer.Bind();

   glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   Skybox_pass(SkyboxVAO, skyboxShader, skybox);

   drawScene_pass(shader,  backpack, skybox);

   outline_pass(outlineShader, backpack);

   DrawToScreen(ScreenVAO, ColourBuffer->id, framebufferShader);

   // Reset stencil 
   glStencilMask(0xFF);
   glStencilFunc(GL_ALWAYS, 1, 0xFF);   
   glEnable(GL_DEPTH_TEST); 

   glfwSwapBuffers(window);
   glfwPollEvents();
}


delete ColourBuffer;
delete depthTexture;
glfwTerminate();

return 0;


}