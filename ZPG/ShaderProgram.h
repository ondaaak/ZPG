#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include <glm/glm.hpp>

class ShaderProgram {
public:
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader); 
    bool setShaderProgram();
    void SetUniform(const char* name, const glm::mat4& matrix);
    ~ShaderProgram();

private:
    GLuint id;
};
