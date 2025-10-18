#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>

class Shader {
public:
    Shader(GLenum type, const char* source);
    Shader(GLenum type, const std::string& filename);
    GLuint getId() const;
    ~Shader();

    void createShader(GLenum shaderType, const char* shaderCode);
    void createShaderFromFile(GLenum shaderType, const char* shaderFile);
    void attachShader(GLuint idShaderProgram);

private:
    GLuint id;
};
