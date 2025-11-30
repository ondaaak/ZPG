#pragma once

#include "Camera.h"
#include "Scene.h"
#include <GLFW/glfw3.h>
#include <vector>  

class Controller {
public:
    Controller(Camera* camera, GLFWwindow* window, Scene* scene);
    void processInput(float deltaTime);
    void setActiveScene(Scene* scene);


private:
    Camera* camera;
    GLFWwindow* window;
    Scene* activeScene;

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

    static bool rightMousePressed;
    static double lastX, lastY;
    static bool firstMouse;

    // --- PØIDÁME JEN TOTO ---
    std::vector<glm::vec3> controlPoints;
    DrawableObject* bezierObject = nullptr;
    Model* pointMarkerModel = nullptr;
    ShaderProgram* markerShader = nullptr;
    // --- KONEC PØIDANÉ ÈÁSTI ---
};