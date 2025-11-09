#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include <glm/glm.hpp>
#include "Camera.h"
#include "Light.h"
#include "SpotLight.h"
#include <vector> 

class Camera;

class ShaderProgram : public Observer {
public:
    ShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
    bool setShaderProgram();
    void SetUniform(const char* name, int value);
    void SetUniform(const char* name, float value);
    void SetUniform(const char* name, const glm::vec3& value);
    void SetUniform(const char* name, const glm::mat4& value);

    bool HasUniform(const char* name) const;

    void setLightUniforms(const std::vector<Light*>& lights);


    void setLightsPointer(const std::vector<Light*>* lights) { allLights = lights; }
    void onSubjectChanged(const Subject* subject) override;

    ~ShaderProgram();

private:
    GLuint id;
    bool hasLight = false;

    const std::vector<Light*>* allLights = nullptr;
};