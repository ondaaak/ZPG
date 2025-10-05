// C++
#include "Transformation.h"

Transformation::Transformation()
    : position(0.0f), scale(1.0f), angle(0.0f), axis(0.0f, 0.0f, 1.0f) {}

void Transformation::setPosition(const glm::vec3& pos) { position = pos; }
void Transformation::setRotation(float a, const glm::vec3& ax) { angle = a; axis = ax; }
void Transformation::setScale(const glm::vec3& s) { scale = s; }

glm::mat4 Transformation::getMatrix() const {
    glm::mat4 mat = glm::mat4(1.0f);
    mat = glm::translate(mat, position);
    mat = glm::rotate(mat, angle, axis);
    mat = glm::scale(mat, scale);
    return mat;
}
