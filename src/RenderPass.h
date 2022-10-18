#pragma once
#include <string>
#include "shader.h"

class RenderPass {
public :
    virtual void Render() = 0;
    virtual void Setup() = 0;

    RenderPass(const std::string& name, Shader& shader) : m_name(name), m_shader(shader) {}
    ~RenderPass(){}

protected:
    const std::string& m_name;
    Shader& m_shader;
};