#pragma once
#include <GL/glew.h>

class Model {
public:
    Model(const float* vertices, size_t vertexCount);
    ~Model();

    void draw() const;

private:
    GLuint VAO;
    GLuint VBO;
    GLsizei count;
};
