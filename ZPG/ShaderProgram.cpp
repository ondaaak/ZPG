#include "ShaderProgram.h"
#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>

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

void ShaderProgram::SetUniform(const char* name, const glm::mat4& matrix) {
    glUseProgram(id);
    GLint idModelTransform = glGetUniformLocation(id, "modelMatrix");
    if (idModelTransform == -1) {
        fprintf(stderr, "Uniform %s not found in shader!\n", name);
        return;
    }
    glUseProgram(id);
    glUniformMatrix4fv(idModelTransform, 1, GL_FALSE, glm::value_ptr(matrix));
}


ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);
}

