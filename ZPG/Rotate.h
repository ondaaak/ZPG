#pragma once
#include "Transformation.h"
class Rotate : public Transformation {
public:
    Rotate(float angle, const glm::vec3& axis) : angle(angle), axis(axis) {}

    glm::mat4 getMatrix() const override {
        return glm::rotate(glm::mat4(1.0f), angle, axis);
    }
	void setAngle(float newAngle) { angle = newAngle; }

private:
    float angle;
    glm::vec3 axis;
};