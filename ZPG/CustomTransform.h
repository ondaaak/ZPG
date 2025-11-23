#pragma once
#include "Transformation.h"
#include <glm/glm.hpp>

class CustomTransform : public Transformation {
public:
    CustomTransform(float number = 1) : w(number) {}

    glm::mat4 getMatrix() const override {
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix[3][3] = w;
        return matrix;
    }

    virtual Transformation* clone() const override {
        return new CustomTransform(w);
    }

private:
    float w;
};
