#pragma once 
#include <glm/glm.hpp>
#include "RenderPass.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "../model.h"
#include "../Primitives/texture.h"
#include "../Primitives/camera.h"

class OutlinePass : RenderPass {

public:
    void Render() override;
    
    OutlinePass (Model& sceneObject, glm::mat4& model , glm::mat4& projection, Camera& camera) ;
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