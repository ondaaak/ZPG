#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class ShaderProgram; 

class Camera {
public:
    Camera(ShaderProgram* shaderProgram, glm::vec3 eye = glm::vec3(0.0f, 0.0f, 2.0f));
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;
    void processKeyboard(int key, float deltaTime);
    void processMouse(float xoffset, float yoffset);

    void addObserver(ShaderProgram* observer);
    void notifyObservers();

    void setAspectRatio(float ratio) { aspectRatio = ratio; }

    glm::vec3 getEye() const { return eye; }
    glm::vec3 getTarget() const { return target; }
    glm::vec3 getUp() const { return up; }

private:
    glm::vec3 eye;
    glm::vec3 target;
    glm::vec3 up;
    float alpha, fi; 
    float speed;
    std::vector<ShaderProgram*> observers;
    float aspectRatio;
};
