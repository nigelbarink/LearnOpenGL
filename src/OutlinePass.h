#pragma once 
#include <glm/glm.hpp>
#include "RenderPass.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "texture.h"
#include "model.h"
#include "camera.h"

class OutlinePass : RenderPass {

public:
    void Setup()  override;
    void Render() override;
    
    OutlinePass (Shader& shader, Model& sceneObject, glm::mat4& model , glm::mat4& projection, Camera& camera) ;
    ~OutlinePass();

private:
    FrameBuffer m_outlineFrameBuffer;
    RenderBuffer m_outlineRenderBuffer;
    Texture* m_depthTexture;

    glm::mat4 model;
    glm::mat4 projection;

    Model& m_model;
    Camera& camera;

};