#pragma once
#include <glm/glm.hpp>

class Transformation {
public:
    virtual ~Transformation() {}
    virtual glm::mat4 getMatrix() const = 0;
};
