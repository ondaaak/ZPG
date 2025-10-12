#include "Camera.h"
#include "ShaderProgram.h"
#include <GLFW/glfw3.h>
#include <cmath>

Camera::Camera(ShaderProgram* shaderProgram, glm::vec3 eyePos)
    : eye(eyePos), up(0.0f, 1.0f, 0.0f), alpha(glm::radians(90.0f)), fi(glm::radians(-90.0f)), speed(2.0f)
{
    target.x = sin(alpha) * cos(fi);
    target.z = sin(alpha) * sin(fi);
    target.y = cos(alpha);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(eye, eye + target, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 100.0f);
}

void Camera::processKeyboard(int key, float deltaTime) {
    glm::vec3 right = glm::normalize(glm::cross(target, up));
    float velocity = speed * deltaTime;
    if (key == GLFW_KEY_W) eye += target * velocity;
    if (key == GLFW_KEY_S) eye -= target * velocity;
    if (key == GLFW_KEY_A) eye -= right * velocity;
    if (key == GLFW_KEY_D) eye += right * velocity;
    notifyObservers();
}

void Camera::processMouse(float xoffset, float yoffset) {
    float sensitivity = 0.01f;
    fi += xoffset * sensitivity;
    alpha += yoffset * sensitivity;
    
    if (alpha < 0.1f) alpha = 0.1f;
    if (alpha > 3.13f) alpha = 3.13f;
    
    target.x = sin(alpha) * cos(fi);
    target.z = sin(alpha) * sin(fi);
    target.y = cos(alpha);
    
    target = glm::normalize(target);
    notifyObservers();
}

void Camera::addObserver(ShaderProgram* observer) {
    observers.push_back(observer);
}

void Camera::notifyObservers() {
    for (auto* obs : observers) {
        obs->onCameraChanged(this, aspectRatio);
    }
}
