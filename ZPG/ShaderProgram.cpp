#include "ShaderProgram.h"
#include <stdio.h>

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) {
    id = glCreateProgram();
    glAttachShader(id, vertexShader.getId()); 
    glAttachShader(id, fragmentShader.getId());
    glLinkProgram(id);

    GLint success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER_PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }
}

bool ShaderProgram::setShaderProgram() {
    glUseProgram(id);
    return true;
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);
}
