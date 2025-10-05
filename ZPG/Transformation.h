// C++
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transformation {
public:
    Transformation();

    void setPosition(const glm::vec3& position);
    void setRotation(float angle, const glm::vec3& axis);
    void setScale(const glm::vec3& scale);

    glm::mat4 getMatrix() const;

private:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 axis;
    float angle;
};
