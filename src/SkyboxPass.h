#pragma once
#include "RenderPass.h"
#include "Renderer.h"
#include "Skybox.h"
#include "shader.h"

class SkyboxPass : RenderPass { 
    public:
        void Render() override;
        void Setup() override;

        SkyboxPass(Shader& shader, glm::mat4& projection, Skybox& skybox, Camera& camera) 
        : RenderPass("Render Pass - Skybox", shader), projection(projection), skybox(skybox), camera(camera){}
        ~SkyboxPass();
        
    private:
        Skybox skybox;
        glm::mat4& projection;
        Camera& camera;

};