#pragma once
#include "Light.h"
#include <cmath> // P�id�no pro cos()

class SpotLight : public Light {
public:
    // ZM�NA: Konstruktor te� bere �hel, ale ukl�d� si jeho COS quantifiable
    SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float cutoffAngle_radians)
        : Light(position, color), direction(direction), cutoff(cos(cutoffAngle_radians)) {
    }
    glm::vec3 getDirection() const { return direction; }
    void setDirection(const glm::vec3& dir) { direction = dir; notifyObservers(this); }

    // Vrac� cosinus �hlu
    float getCutoff() const { return cutoff; }

    // Bere �hel v radi�nech, ukl�d� cosinus
    void setCutoff(float cutoffAngle_radians) {
        cutoff = cos(cutoffAngle_radians);
        notifyObservers(this);
    }
protected:
    glm::vec3 direction;
    float cutoff; // Nyn� ukl�d� cos(�hlu), ne �hel
};