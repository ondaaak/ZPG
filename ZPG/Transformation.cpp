#include "Transformation.h"

Transformation::Transformation()
    : position(0.0f), scaleVec(1.0f), rotationAxis(0.0f, 0.0f, 1.0f), rotationAngle(0.0f) {
}

void Transformation::translate(const glm::vec3& offset) {
    position += offset;
}

void Transformation::rotate(float angle, const glm::vec3& axis) {
    rotationAngle += angle;
    rotationAxis = axis;
}

void Transformation::scale(const glm::vec3& factor) {
    scaleVec *= factor;
}

void Transformation::setPosition(const glm::vec3& pos) {
    position = pos;
}

void Transformation::setRotation(float angle, const glm::vec3& axis) {
    rotationAngle = angle;
    rotationAxis = axis;
}

void Transformation::setScale(const glm::vec3& scale) {
    scaleVec = scale;
}

void Transformation::reset() {
    position = glm::vec3(0.0f);
    scaleVec = glm::vec3(1.0f);
    rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
    rotationAngle = 0.0f;
}

glm::mat4 Transformation::getMatrix() const {
    glm::mat4 mat = glm::mat4(1.0f);
    mat = glm::translate(mat, position);
    mat = glm::rotate(mat, rotationAngle, rotationAxis);
    mat = glm::scale(mat, scaleVec);
    return mat;
}
