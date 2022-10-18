#pragma once 
#include "RenderPass.h"
#include "Renderer.h"
#include "Skybox.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
class ScenePass : RenderPass {

    public:
        void Setup() override;
        void Render() override;

        ScenePass(Shader& shader, Model& scene, glm::mat4& model, glm::mat4& projection, Camera& camera) 
        : RenderPass("RenderPass - Scene", shader), m_scene(scene), model(model), projection(projection), camera(camera) {}
        ~ScenePass();

        void SetSkybox(Skybox& skybox);

    private:
        Skybox* m_skybox;
        Model& m_scene;
        glm::mat4& model;
        glm::mat4& projection;
        Camera& camera;


};