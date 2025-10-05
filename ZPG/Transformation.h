#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transformation {
public:
    Transformation();

    void translate(const glm::vec3& offset);
    void rotate(float angle, const glm::vec3& axis);
    void scale(const glm::vec3& factor);

    void setPosition(const glm::vec3& position);
    void setRotation(float angle, const glm::vec3& axis);
    void setScale(const glm::vec3& scale);

    void reset();

    glm::mat4 getMatrix() const;

private:
    glm::vec3 position;
    glm::vec3 scaleVec;
    glm::vec3 rotationAxis;
    float rotationAngle;
};
