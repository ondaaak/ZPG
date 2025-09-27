#pragma once
#include <GL/glew.h>
#include "ShaderProgram.h"

class DrawableObject {
public:
    DrawableObject(const float* vertices, size_t vertexCount, ShaderProgram* shaderProgram);
    ~DrawableObject();
    void render() const;

private:
    ShaderProgram* shaderProgram;
    GLuint VBO;
    GLuint VAO;
    GLsizei count;
};
