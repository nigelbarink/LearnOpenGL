#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

#include "stb_image.h"


class Cubemap {
public:

    unsigned int id;

    Cubemap();
    ~Cubemap();

    void Bind();
    void Unbind();

    void loadCubeTextures(const std::vector<std::string>& texture_faces);


};  


