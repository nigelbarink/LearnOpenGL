#pragma once
#include <string>
#include "../Primitives/shader.h"

class RenderPass {
public :
    virtual void Render() = 0;

    RenderPass(const std::string& name,const std::string& VertexShaderFile, const std::string& FragmentShaderFile) : m_shader(VertexShaderFile.c_str(), FragmentShaderFile.c_str())
    {
        std::cout << "Created " << name << std::endl;
    }
    
    ~RenderPass(){}

protected:
    Shader m_shader;
 
};