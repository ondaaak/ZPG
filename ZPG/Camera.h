#pragma once
#include "Subject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera : public Subject {
public:
    Camera(glm::vec3 eye = glm::vec3(0.0f, 0.0f, 2.0f));
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;
    void processKeyboard(int key, float deltaTime);
    void processMouse(float xoffset, float yoffset);
    void setAspectRatio(float ratio);

private:
    glm::vec3 eye, target, up;
    float alpha, fi, speed, aspectRatio;
};
