#include "ShaderProgram.h"
#include <stdio.h>

ShaderProgram::ShaderProgram(GLuint vertexShader, GLuint fragmentShader)
    : id(0)
{
    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
}

bool ShaderProgram::setShaderProgram() {
    GLint status;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(id, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
        return false;
    }
    glUseProgram(id);
    return true;
}

ShaderProgram::~ShaderProgram() {
    if (id) glDeleteProgram(id);
}
