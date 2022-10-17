#include "Cubemap.h"


Cubemap::Cubemap(){
    glGenTextures(1,&id);
}

Cubemap::~Cubemap(){
   // glDeleteTextures(id);
}

void Cubemap::Bind(){
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

};

void Cubemap::Unbind(){
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::loadCubeTextures(const std::vector<std::string>& texture_faces){
    int width, height, nrChannels;
    unsigned char* data;
    stbi_set_flip_vertically_on_load(false);

    for (unsigned int i = 0; i < texture_faces.size(); i++){
        data = stbi_load(texture_faces[i].c_str(), &width, &height, &nrChannels,0);
        if (data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0 ,GL_RGB, GL_UNSIGNED_BYTE,data);
        } else {
            std::cout << "Cubemap tex failed to load at path: " << texture_faces[i] << std::endl;
        }
    
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  


    stbi_set_flip_vertically_on_load(true);

}