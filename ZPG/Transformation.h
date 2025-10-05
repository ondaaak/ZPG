#pragma once
#include <glm/glm.hpp>

class Transformation {
public:
    Transformation();

    void translate(const glm::vec3& offset);
    void rotate(float angle, const glm::vec3& axis);
    void scale(const glm::vec3& factor);

    glm::mat4 getMatrix() const;

private:
    glm::mat4 matrix;
};