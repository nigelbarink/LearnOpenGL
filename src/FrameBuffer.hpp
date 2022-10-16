#pragma once
#include <glad/glad.h>
#include "texture.h"
#include "RenderBuffer.hpp"


class FrameBuffer 
{
public:
    GLuint id;
    void Bind();
    void Unbind();

    void Attach(const Texture& texture);
    void Attach(const RenderBuffer& renderbuffer);
    bool IsComplete();
    

    FrameBuffer();
    ~FrameBuffer();

};
