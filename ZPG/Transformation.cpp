#include "Transformation.h"
#include <glm/gtc/matrix_transform.hpp>

Transformation::Transformation()
    : matrix(1.0f) {
}

void Transformation::translate(const glm::vec3& offset) {
    matrix = glm::translate(matrix, offset);
}

void Transformation::rotate(float angle, const glm::vec3& axis) {
    matrix = glm::rotate(matrix, angle, axis);
}

void Transformation::scale(const glm::vec3& factor) {
    matrix = glm::scale(matrix, factor);
}

glm::mat4 Transformation::getMatrix() const {
    return matrix;
}
