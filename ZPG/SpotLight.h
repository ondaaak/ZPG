#pragma once
#include "Light.h"
#include <cmath> // Pro cos()

class SpotLight : public Light {
public:
    // ZMÌNA: Konstruktor bere vnitøní a vnìjší úhel
    SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float innerAngle_radians, float outerAngle_radians)
        : Light(position, color), direction(direction),
        innerCutoff(cos(innerAngle_radians)),
        outerCutoff(cos(outerAngle_radians)) {
    }

    glm::vec3 getDirection() const { return direction; }
    void setDirection(const glm::vec3& dir) { direction = dir; notifyObservers(this); }

    // ZMÌNA: Gettery pro oba cosiny úhlù
    float getInnerCutoff() const { return innerCutoff; }
    float getOuterCutoff() const { return outerCutoff; }

    // ZMÌNA: Setter pro oba úhly
    void setCutoffs(float innerAngle_radians, float outerAngle_radians) {
        innerCutoff = cos(innerAngle_radians);
        outerCutoff = cos(outerAngle_radians);
        notifyObservers(this);
    }

protected:
    glm::vec3 direction;
    // ZMÌNA: Ukládáme dva cosiny
    float innerCutoff; // Cosinus vnitøního, jasného kužele (vìtší hodnota)
    float outerCutoff; // Cosinus vnìjšího kužele (menší hodnota)
};