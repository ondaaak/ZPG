#pragma once
#include "Light.h"

class AmbientLight : public Light {
public:
    AmbientLight(const glm::vec3& color) : Light(glm::vec3(0.0f), color) {}
};