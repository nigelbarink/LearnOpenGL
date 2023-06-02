#include "Skybox.h"

void Skybox::Bind()
{
    glBindVertexArray(m_skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
};

void Skybox::Unbind()
{
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::Destroy() {
    glDeleteTextures(1, &id);
    std::cout << "Skybox destroyed!" << std::endl;
}


void Skybox::loadCubeTextures(const std::vector<std::string>& texture_faces)
{
    glGenTextures(1, &id);

    // Create SkyboxVAO

    glGenVertexArrays(1, &m_skyboxVAO);
    glBindVertexArray(m_skyboxVAO);

    glGenBuffers(1, &m_skyboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);

    glBufferData(GL_ARRAY_BUFFER, m_skyboxVertices.size() * sizeof(float), &m_skyboxVertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);

    std::cout << "Skybox Created" << std::endl;


    Bind();
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
    Unbind();

}