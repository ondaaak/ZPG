#pragma once

#include "Light.h"

class DirectionalLight : public Light {
public:
    DirectionalLight(const glm::vec3& direction, const glm::vec3& color)
        : Light(glm::vec3(0.0f), color), direction(direction) {
    }
    glm::vec3 getDirection() const { return direction; }
    void setDirection(const glm::vec3& dir) { direction = dir; notifyObservers(this); }
protected:
    glm::vec3 direction;
};