#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
#include "ShaderProgram.h"
#include "stb_image.h"
#include <iostream>
#include <glm/glm.hpp>

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces, ShaderProgram* shader);
    ~Skybox();

    void draw() const;

private:
    GLuint skyboxVAO, skyboxVBO;
    GLuint cubemapTexture;
    ShaderProgram* shaderProgram;

    void loadCubemap(const std::vector<std::string>& faces);
};