#pragma once
#include "RenderPass.h"
#include "../Primitives/Skybox.h"
#include "../Primitives/shader.h"
#include "../Primitives/camera.h"

class SkyboxPass : public RenderPass { 
    public:
        void Render() override;

        SkyboxPass( Skybox& skybox, Camera& camera, glm::mat4& projection);
        ~SkyboxPass() = default;
        
    private:
        Skybox& skybox;
        Camera& camera;
        glm::mat4& projection;


};