#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include <glm/glm.hpp>

class ShaderProgram {
public:
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader); 
    bool setShaderProgram();
    void SetUniform(const char* name, int value);
    void SetUniform(const char* name, float value);
    void SetUniform(const char* name, const glm::vec3& value);
    void SetUniform(const char* name, const glm::mat4& value);
    ~ShaderProgram();
    
private:
    GLuint id;
};
