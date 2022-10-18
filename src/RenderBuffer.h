#pragma once 
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class RenderBuffer
{
public:
    unsigned int id;

    void Bind();
    void Unbind();

    void UseDepthAndStencil();
    
    RenderBuffer();
    ~RenderBuffer();


};