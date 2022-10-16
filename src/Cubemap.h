#pragma once
#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>
#include <vector>
#include <string>


class Cubemap {
public:

    unsigned int id;

    Cubemap();
    ~Cubemap();

    void Bind();
    void Unbind();

    void loadCubeTextures(const std::vector<std::string>& texture_faces);


};  


