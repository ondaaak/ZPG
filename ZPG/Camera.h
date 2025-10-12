#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class ShaderProgram; // dop�edn� deklarace

class Camera {
public:
    Camera(ShaderProgram* shaderProgram, glm::vec3 eye = glm::vec3(0.0f, 0.0f, 2.0f));
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;
    void processKeyboard(int key, float deltaTime);
    void processMouse(float xoffset, float yoffset);

    // Observer pattern
    void addObserver(ShaderProgram* observer);
    void notifyObservers();

    // Gettery pro prom�nn�
    glm::vec3 getEye() const { return eye; }
    glm::vec3 getTarget() const { return target; }
    glm::vec3 getUp() const { return up; }

private:
    glm::vec3 eye;
    glm::vec3 target;
    glm::vec3 up;
    float alpha, fi; // �hly pro v�po�et targetu
    float speed;
    ShaderProgram* m_shaderProgram;
    std::vector<ShaderProgram*> observers;
};
