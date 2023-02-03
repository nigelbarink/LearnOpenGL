#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

struct Texture{
   GLuint id;
   std::string type;
   std::string path;
};

Texture* CreateTexture(unsigned int width, unsigned int height);