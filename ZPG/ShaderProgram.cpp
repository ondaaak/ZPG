#include "ShaderProgram.h"
#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>

// P¯id·no pro dynamic_cast
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "SpotLight.h"


ShaderProgram::ShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderFile);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderFile);

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

// ODSTRANÃNY starÈ setLightUniforms(const std::vector<Light>& lights)
// a setLightUniforms(const std::vector<SpotLight>& lights)

// PÿID¡NA NOV¡ POLYMORFNÕ VERZE
void ShaderProgram::setLightUniforms(const std::vector<Light*>& lights) {
    glUseProgram(id); // UjistÌme se, ûe pouûÌv·me tento program
    SetUniform("numLights", static_cast<int>(lights.size()));

    for (size_t i = 0; i < lights.size(); ++i) {
        if (lights[i] == nullptr) continue; // BezpeËnostnÌ kontrola

        char namebuf[64];
        const Light* light = lights[i];

        // SpoleËnÈ vlastnosti
        snprintf(namebuf, sizeof(namebuf), "lights[%zu].position", i);
        SetUniform(namebuf, light->getPosition());
        snprintf(namebuf, sizeof(namebuf), "lights[%zu].diffuse", i);
        SetUniform(namebuf, light->getDiffuse());
        snprintf(namebuf, sizeof(namebuf), "lights[%zu].specular", i);
        SetUniform(namebuf, light->getSpecular());

        // PolymorfnÌ Ë·st - zjiöùov·nÌ typu

        // Typ 1: SpotLight
        if (const SpotLight* sl = dynamic_cast<const SpotLight*>(light)) {
            snprintf(namebuf, sizeof(namebuf), "lights[%zu].direction", i);
            SetUniform(namebuf, sl->getDirection());

            // Toto uû bylo spr·vnÏ:
            snprintf(namebuf, sizeof(namebuf), "lights[%zu].innerCutoff", i);
            SetUniform(namebuf, sl->getInnerCutoff());
            snprintf(namebuf, sizeof(namebuf), "lights[%zu].outerCutoff", i);
            SetUniform(namebuf, sl->getOuterCutoff());

            snprintf(namebuf, sizeof(namebuf), "lights[%zu].type", i);
            SetUniform(namebuf, 1); // 1 = SpotLight
        }
        // Typ 2: DirectionalLight
        else if (const DirectionalLight* dl = dynamic_cast<const DirectionalLight*>(light)) {
            snprintf(namebuf, sizeof(namebuf), "lights[%zu].direction", i);
            SetUniform(namebuf, dl->getDirection());

            // OPRAVA: Nahrazeno 'cutoff' za 'innerCutoff' a 'outerCutoff'
            snprintf(namebuf, sizeof(namebuf), "lights[%zu].innerCutoff", i);
            SetUniform(namebuf, -1.0f); // NenÌ relevantnÌ
            snprintf(namebuf, sizeof(namebuf), "lights[%zu].outerCutoff", i);
            SetUniform(namebuf, -1.0f); // NenÌ relevantnÌ

            snprintf(namebuf, sizeof(namebuf), "lights[%zu].type", i);
            SetUniform(namebuf, 2); // 2 = DirectionalLight
        }
        // Typ 3: AmbientLight
        else if (dynamic_cast<const AmbientLight*>(light)) {
            snprintf(namebuf, sizeof(namebuf), "lights[%zu].direction", i);
            SetUniform(namebuf, glm::vec3(0.0f)); // NenÌ relevantnÌ

            // OPRAVA: Nahrazeno 'cutoff' za 'innerCutoff' a 'outerCutoff'
            snprintf(namebuf, sizeof(namebuf), "lights[%zu].innerCutoff", i);
            SetUniform(namebuf, -1.0f); // NenÌ relevantnÌ
            snprintf(namebuf, sizeof(namebuf), "lights[%zu].outerCutoff", i);
            SetUniform(namebuf, -1.0f); // NenÌ relevantnÌ

            snprintf(namebuf, sizeof(namebuf), "lights[%zu].type", i);
            SetUniform(namebuf, 3); // 3 = AmbientLight
        }
        // Typ 0: PointLight (z·kladnÌ t¯Ìda)
        else {
            snprintf(namebuf, sizeof(namebuf), "lights[%zu].direction", i);
            SetUniform(namebuf, glm::vec3(0.0f)); // NenÌ relevantnÌ

            // OPRAVA: Nahrazeno 'cutoff' za 'innerCutoff' a 'outerCutoff'
            snprintf(namebuf, sizeof(namebuf), "lights[%zu].innerCutoff", i);
            SetUniform(namebuf, -1.0f); // NenÌ relevantnÌ
            snprintf(namebuf, sizeof(namebuf), "lights[%zu].outerCutoff", i);
            SetUniform(namebuf, -1.0f); // NenÌ relevantnÌ

            snprintf(namebuf, sizeof(namebuf), "lights[%zu].type", i);
            SetUniform(namebuf, 0); // 0 = PointLight
        }
    }
    hasLight = true;
}


void ShaderProgram::onSubjectChanged(const Subject* subject) {

    const Camera* camera = dynamic_cast<const Camera*>(subject);
    if (camera) {
        SetUniform("viewMatrix", camera->getViewMatrix());
        SetUniform("projectMatrix", camera->getProjectionMatrix());

        if (hasLight) {
            SetUniform("viewPos", camera->getCameraPosition());
        }
        return;
    }


    // OPRAVA: Odkomentov·no a funkËnÌ
    const Light* light = dynamic_cast<const Light*>(subject);
    if (light && allLights) {
        // SvÏtlo se zmÏnilo, znovu nastavÌme VäECHNA svÏtla v seznamu
        setLightUniforms(*allLights);
        return;
    }
}