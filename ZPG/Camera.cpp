#include "Camera.h"
#include "ShaderProgram.h"
#include <GLFW/glfw3.h>

Camera::Camera(glm::vec3 pos)
    : position(pos), yaw(-90.0f), pitch(0.0f), speed(2.5f) {}

glm::mat4 Camera::getViewMatrix() const {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    return glm::lookAt(position, position + glm::normalize(front), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::processKeyboard(int key, float deltaTime) {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

    float velocity = speed * deltaTime;
    if (key == GLFW_KEY_W) position += front * velocity;
    if (key == GLFW_KEY_S) position -= front * velocity;
    if (key == GLFW_KEY_A) position -= right * velocity;
    if (key == GLFW_KEY_D) position += right * velocity;
    notifyObservers();
}

void Camera::processMouse(float xoffset, float yoffset) {
    float sensitivity = 0.1f;
    yaw += xoffset * sensitivity;
    pitch -= yoffset * sensitivity;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    notifyObservers();
}

// Observer pattern
void Camera::addObserver(ShaderProgram* observer) {
    observers.push_back(observer);
}

void Camera::notifyObservers() {
    for (auto* obs : observers) {
        obs->onCameraChanged(this);
    }
}
