#pragma once
#include "Light.h"
#include <cmath> 

class SpotLight : public Light {
public:
    SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float innerAngle_radians, float outerAngle_radians)
        : Light(position, color), direction(direction),
        innerCutoff(cos(innerAngle_radians)),
        outerCutoff(cos(outerAngle_radians)) {
    }

    glm::vec3 getDirection() const { return direction; }
    void setDirection(const glm::vec3& dir) { direction = dir; notifyObservers(this); }

    float getInnerCutoff() const { return innerCutoff; }
    float getOuterCutoff() const { return outerCutoff; }

    void setCutoffs(float innerAngle_radians, float outerAngle_radians) {
        innerCutoff = cos(innerAngle_radians);
        outerCutoff = cos(outerAngle_radians);
        notifyObservers(this);
    }

protected:
    glm::vec3 direction;
    float innerCutoff; 
    float outerCutoff; 
};