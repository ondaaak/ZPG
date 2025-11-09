#pragma once
#include <GL/glew.h>
#include <iostream>

class Model {
public:
    Model(const float* vertices, size_t vertexCount, int format);
    Model(const char* name);
    ~Model();

    void draw() const;

private:
    GLuint VAO;
    GLuint VBO;
    GLsizei count;
};
