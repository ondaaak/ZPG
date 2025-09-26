#pragma once
#include <GL/glew.h>

class ShaderProgram {
public:
    ShaderProgram(GLuint vertexShader, GLuint fragmentShader);
    bool setShaderProgram();
    ~ShaderProgram();

private:
    GLuint id;
};
