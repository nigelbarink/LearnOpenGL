#pragma once
#include "../Primitives/texture.h"
#include "RenderBuffer.h"


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
