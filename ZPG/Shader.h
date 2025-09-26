#pragma once
#include <GL/glew.h>

class Shader {
public:
    Shader(GLenum type, const char* source);
    GLuint getId() const;
    ~Shader();

private:
    GLuint id;
};
