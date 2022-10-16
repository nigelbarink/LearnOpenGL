#pragma once
#include <string>
#include <glad/glad.h>

struct Texture{
   GLuint id;
   std::string type;
   std::string path;
};


Texture* CreateTexture(unsigned int width, unsigned int height);