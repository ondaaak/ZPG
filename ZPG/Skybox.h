#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
#include "ShaderProgram.h"
#include "stb_image.h"
#include <iostream>

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
    ~Skybox();

    void draw();

    ShaderProgram* getShader();

private:
    GLuint skyboxVAO, skyboxVBO;
    GLuint cubemapTexture;
    ShaderProgram* shaderProgram;

    void loadCubemap(const std::vector<std::string>& faces);
};