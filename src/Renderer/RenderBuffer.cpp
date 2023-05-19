#include "RenderBuffer.h"

void RenderBuffer::UseDepthAndStencil (int width, int height){
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); /// NOTE: should be part of creating but kept seperate for now!
}

void RenderBuffer::Bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void RenderBuffer::Unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

RenderBuffer::RenderBuffer()
{
    glGenRenderbuffers(1, &id);
}

RenderBuffer::~RenderBuffer()
{
   // glDeleteRenderbuffers(GL_RENDERBUFFER, &id);
}
