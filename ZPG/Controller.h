#pragma once
#include "Camera.h"

#include "Scene.h"

// Dùležité pro unProject
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Controller {
public:
    // Konstruktor nyní pøijímá i scénu
    Controller(Camera* camera, GLFWwindow* window, Scene* scene);

    void processInput(float deltaTime);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

private:
    Camera* camera;
    GLFWwindow* window;
    Scene* activeScene; // Ukazatel na aktivní scénu pro pøidávání objektù

    // Statické promìnné pro správu stavu myši
    static bool rightMousePressed;
    static double lastX, lastY;
    static bool firstMouse;
};