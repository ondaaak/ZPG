#pragma once
#include "Transformation.h"

class Translate : public Transformation {
public:
    Translate(const glm::vec3& offset) : offset(offset) {}

    glm::mat4 getMatrix() const override {
        return glm::translate(glm::mat4(1.0f), offset);
    }

    void setOffset(const glm::vec3& newOffset) {
        offset = newOffset;
    }

    virtual Transformation* clone() const override {
        return new Translate(offset);
    }

private:
    glm::vec3 offset;
};