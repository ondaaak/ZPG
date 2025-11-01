#pragma once
#include "Light.h"
#include <cmath> // Pro cos()

class SpotLight : public Light {
public:
    // ZM�NA: Konstruktor bere vnit�n� a vn�j�� �hel
    SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float innerAngle_radians, float outerAngle_radians)
        : Light(position, color), direction(direction),
        innerCutoff(cos(innerAngle_radians)),
        outerCutoff(cos(outerAngle_radians)) {
    }

    glm::vec3 getDirection() const { return direction; }
    void setDirection(const glm::vec3& dir) { direction = dir; notifyObservers(this); }

    // ZM�NA: Gettery pro oba cosiny �hl�
    float getInnerCutoff() const { return innerCutoff; }
    float getOuterCutoff() const { return outerCutoff; }

    // ZM�NA: Setter pro oba �hly
    void setCutoffs(float innerAngle_radians, float outerAngle_radians) {
        innerCutoff = cos(innerAngle_radians);
        outerCutoff = cos(outerAngle_radians);
        notifyObservers(this);
    }

protected:
    glm::vec3 direction;
    // ZM�NA: Ukl�d�me dva cosiny
    float innerCutoff; // Cosinus vnit�n�ho, jasn�ho ku�ele (v�t�� hodnota)
    float outerCutoff; // Cosinus vn�j��ho ku�ele (men�� hodnota)
};