#include "RenderBuffer.h"

void RenderBuffer::UseDepthAndStencil (){
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); /// NOTE: should be part of creating but kept seperate for now!
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
    glDeleteRenderbuffers(GL_RENDERBUFFER, &id);
}
