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

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);
}

void ShaderProgram::SetUniform(const char* name, const glm::mat4& value) {
    glUseProgram(id);
    GLint loc = glGetUniformLocation(id, name);
    if (loc == -1) {
        fprintf(stderr, "Uniform %s not found in shader!\n", name);
        return;
    }
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::SetUniform(const char* name, int value) {
    glUseProgram(id);
    GLint loc = glGetUniformLocation(id, name);
    if (loc == -1) {
        fprintf(stderr, "Uniform %s not found in shader!\n", name);
        return;
    }
    glUniform1i(loc, value);
}

void ShaderProgram::SetUniform(const char* name, float value) {
    glUseProgram(id);
    GLint loc = glGetUniformLocation(id, name);
    if (loc == -1) {
        fprintf(stderr, "Uniform %s not found in shader!\n", name);
        return;
    }
    glUniform1f(loc, value);
}

void ShaderProgram::SetUniform(const char* name, const glm::vec3& value) {
    glUseProgram(id);
    GLint loc = glGetUniformLocation(id, name);
    if (loc == -1) {
        fprintf(stderr, "Uniform %s not found in shader!\n", name);
        return;
    }
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void ShaderProgram::onCameraChanged(const Camera* camera, float aspectRatio) {
    SetUniform("viewMatrix", camera->getViewMatrix());
    SetUniform("projectMatrix", camera->getProjectionMatrix(aspectRatio));
	if (hasLight){ 
        SetUniform("viewPos", camera->getCameraPosition()); 
    }
        
}

void ShaderProgram::setLightUniforms(int index, const Light& light) {
    char namebuf[64];

    snprintf(namebuf, sizeof(namebuf), "lights[%d].position", index);
    SetUniform(namebuf, light.getPosition());

    snprintf(namebuf, sizeof(namebuf), "lights[%d].diffuse", index);
    SetUniform(namebuf, light.getDiffuse());

    snprintf(namebuf, sizeof(namebuf), "lights[%d].specular", index);
    SetUniform(namebuf, light.getSpecular());
}

