#pragma once
#include "Subject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera : public Subject {
public:
    Camera(glm::vec3 eye = glm::vec3(0.0f, 0.0f, 1.5f));
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    void processKeyboard(int key, float deltaTime);
    void processMouse(float xoffset, float yoffset);
    void setAspectRatio(float ratio);
	glm::vec3 getCameraPosition() const { return eye; }
	glm::vec3 getCameraFront() const { return target; }
private:
    glm::vec3 eye, target, up;
    float alpha, fi, speed, aspectRatio;
};
