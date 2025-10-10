#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "ShaderProgram.h"

class ShaderProgram; 

class Camera {
public:
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 2.0f));
    glm::mat4 getViewMatrix() const;
    void processKeyboard(int key, float deltaTime);
    void processMouse(float xoffset, float yoffset);

    // Observer pattern
    void addObserver(ShaderProgram* observer);
    void notifyObservers();

    glm::vec3 position;
    float yaw, pitch;
    float speed;
private:
    std::vector<ShaderProgram*> observers;
};
