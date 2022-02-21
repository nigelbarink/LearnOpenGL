#pragma once
#include <string>
#include <vector>

#include "mesh.h"
#include "shader.h"


#include "stb_image.h"

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post Processing flags
class Model {
    public:


    Model(std::string const& path);
    void Draw(Shader& shader);
    private: 
        // model data

        std::vector<Texture> textures_loaded;
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene );
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName );
};
