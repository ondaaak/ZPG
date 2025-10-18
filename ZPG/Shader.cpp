#include "Shader.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(GLenum type, const char* source) {
    createShader(type, source);
}

Shader::Shader(GLenum type, const std::string& filename) {
    createShaderFromFile(type, filename.c_str());
}

void Shader::createShader(GLenum shaderType, const char* shaderCode) {
    id = glCreateShader(shaderType);
    glShaderSource(id, 1, &shaderCode, nullptr);
    glCompileShader(id);

    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
    }
}

void Shader::createShaderFromFile(GLenum shaderType, const char* shaderFile) {
    std::ifstream file(shaderFile);
       if (!file.is_open()) {
        std::cerr << "Unable to open file " << shaderFile << std::endl;
        exit(-1);
    }
    std::string shaderCode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    createShader(shaderType, shaderCode.c_str());
}

void Shader::attachShader(GLuint idShaderProgram) {
    glAttachShader(idShaderProgram, id);
}

Shader::~Shader() {
    glDeleteShader(id);
}

GLuint Shader::getId() const {
    return id;
}
