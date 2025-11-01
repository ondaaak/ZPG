#pragma once
#include "Light.h"
#include <cmath> // Pøidáno pro cos()

class SpotLight : public Light {
public:
    // ZMÌNA: Konstruktor teï bere úhel, ale ukládá si jeho COS quantifiable
    SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float cutoffAngle_radians)
        : Light(position, color), direction(direction), cutoff(cos(cutoffAngle_radians)) {
    }
    glm::vec3 getDirection() const { return direction; }
    void setDirection(const glm::vec3& dir) { direction = dir; notifyObservers(this); }

    // Vrací cosinus úhlu
    float getCutoff() const { return cutoff; }

    // Bere úhel v radiánech, ukládá cosinus
    void setCutoff(float cutoffAngle_radians) {
        cutoff = cos(cutoffAngle_radians);
        notifyObservers(this);
    }
protected:
    glm::vec3 direction;
    float cutoff; // Nyní ukládá cos(úhlu), ne úhel
};