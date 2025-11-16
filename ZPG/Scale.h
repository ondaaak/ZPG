#pragma once
#include "Transformation.h"

class Scale : public Transformation {
public:
    Scale(const glm::vec3& scale) : scale(scale) {}

    glm::mat4 getMatrix() const override {
        return glm::scale(glm::mat4(1.0f), scale);
    }

    virtual Transformation* clone() const override {
        return new Scale(scale);
    }

private:
    glm::vec3 scale;
};