#include "FrameBuffer.hpp"


void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBuffer::Attach(const Texture& texture)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);
    
}

void FrameBuffer::Attach(const RenderBuffer& renderbuffer)
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer.id);
}


bool FrameBuffer::IsComplete()
{
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &id );
}


FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &id);
}

