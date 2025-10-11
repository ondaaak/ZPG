#pragma once
#include "Camera.h"
#include <GLFW/glfw3.h>


class Controller {
public:
    Controller(Camera* camera, GLFWwindow* window);

    void processInput(float deltaTime);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

private:
    Camera* camera;
    GLFWwindow* window;
    static bool rightMousePressed;
    static double lastX, lastY;
    static bool firstMouse;
};

