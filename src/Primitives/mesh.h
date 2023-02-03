#pragma once
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "vertex.h"
#include "texture.h"
#include "shader.h"

class Mesh{
   public:
      // Mesh data
      std::vector<Vertex> vertices;
      std::vector<unsigned int> indices; 
      std::vector<Texture> textures;

      Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
      void Draw(Shader &shader);
      bool showOutline;


   private: 
      // render data
      unsigned int VAO, VBO, EBO;

      void setupMesh();    

};