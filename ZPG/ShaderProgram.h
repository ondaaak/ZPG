#pragma once
#include <GL/glew.h>

#include "Shader.h"
#include <glm/glm.hpp>
#include "Camera.h"
#include "Light.h"
class Camera;

class ShaderProgram : public Observer {
public:
    ShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
    bool setShaderProgram();
    void SetUniform(const char* name, int value);
    void SetUniform(const char* name, float value);
    void SetUniform(const char* name, const glm::vec3& value);
    void SetUniform(const char* name, const glm::mat4& value);
	void onCameraChanged(const Camera* camera);
    void setLight(bool option) { hasLight = option; } // remove if not used
	void setLightUniforms(const std::vector<Light>& lights);

    ~ShaderProgram();

private:
    GLuint id;
    bool hasLight = false;
};
