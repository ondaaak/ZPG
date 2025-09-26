// Shader.cpp
#include "Shader.h"
#include <GL/glew.h>
#include <iostream>

Shader::Shader(GLenum type, const char* source) {
    id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);
    // Kontrola kompilace
    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        // Získání a vypsání chybové hlášky
        char infoLog[512];
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
    }
}

Shader::~Shader() {
    glDeleteShader(id);
}

GLuint Shader::getId() const {
    return id;
}
