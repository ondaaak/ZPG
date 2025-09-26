#pragma once
#include <GL/glew.h>
#include "Shader.h"

class ShaderProgram {
public:
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader); 
    bool setShaderProgram();
    ~ShaderProgram();

private:
    GLuint id;
};
