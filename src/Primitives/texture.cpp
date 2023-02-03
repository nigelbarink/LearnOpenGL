#include "texture.h"



Texture* CreateTexture(unsigned int width, unsigned int height)
{
    Texture* result = new Texture();

    glGenTextures(1, &result->id);
    glBindTexture(GL_TEXTURE_2D, result->id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    return result;

}