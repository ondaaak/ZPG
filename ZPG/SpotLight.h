#pragma once
#include "Light.h"


class SpotLight : public Light {
public:
    SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float cutoff)
        : Light(position, color), direction(direction), cutoff(cutoff) {
    }
    glm::vec3 getDirection() const { return direction; }
    void setDirection(const glm::vec3& dir) { direction = dir; notifyObservers(this); }
    float getCutoff() const { return cutoff; }
    void setCutoff(float c) { cutoff = c; notifyObservers(this); }
protected:
    glm::vec3 direction;
    float cutoff;
};
