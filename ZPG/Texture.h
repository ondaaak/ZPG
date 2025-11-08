#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>
#include "stb_image.h"

class Texture {
public:

    Texture(const std::string& path);
    ~Texture();

    void bind(GLuint textureUnit = GL_TEXTURE0);

private:
    GLuint textureID;
};