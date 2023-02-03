#pragma once 
#include "RenderPass.h"
#include "../Primitives/Skybox.h"
#include "../Primitives/shader.h"
#include "../Primitives/camera.h"
#include "../model.h"

class ScenePass : public RenderPass {

    public:
        void Render() override;

        ScenePass(Skybox& skybox,  Model& scene, Camera& camera, glm::mat4 projection , glm::mat4 model) 
        : RenderPass("RenderPass - Scene", "../Shaders/shader.vs", "../Shaders/shader.fs" ), 
        m_scene(scene), camera(camera), projection(projection), model(model), m_skybox(skybox)
        {

            
        }
        ~ScenePass() = default;

        void SetSkybox(Skybox& skybox);

    private:
        Skybox& m_skybox;
        Model& m_scene;
        Camera& camera;

        glm::mat4 projection;
        glm::mat4 model;


};