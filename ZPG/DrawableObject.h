#pragma once
#include <GL/glew.h>

class DrawableObject {
public:
    DrawableObject(const float* vertices, size_t vertexCount);
    ~DrawableObject();
    void draw() const;

private:
    GLuint VBO;
    GLuint VAO;
    GLsizei count;
};
