#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include <glm/glm.hpp>
#include "Camera.h"
class Camera;

class ShaderProgram {
public:
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader); 
    bool setShaderProgram();
    void SetUniform(const char* name, int value);
    void SetUniform(const char* name, float value);
    void SetUniform(const char* name, const glm::vec3& value);
    void SetUniform(const char* name, const glm::mat4& value);
    ~ShaderProgram();
    void onCameraChanged(const Camera* camera); // Observer callback

private:
    GLuint id;
};
