#include "Controller.h"

bool Controller::rightMousePressed = false;
double Controller::lastX = 512;
double Controller::lastY = 400;
bool Controller::firstMouse = true;

Controller::Controller(Camera* cam, GLFWwindow* win)
    : camera(cam), window(win)
{
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
}

void Controller::processInput(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->processKeyboard(GLFW_KEY_W, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->processKeyboard(GLFW_KEY_S, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->processKeyboard(GLFW_KEY_A, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->processKeyboard(GLFW_KEY_D, deltaTime);
}

void Controller::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        rightMousePressed = (action == GLFW_PRESS);
        firstMouse = true;
    }
}

void Controller::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (rightMousePressed) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        float xoffset = float(xpos - lastX);
        float yoffset = float(ypos - lastY);
        lastX = xpos;
        lastY = ypos;

        Controller* ctrl = static_cast<Controller*>(glfwGetWindowUserPointer(window));
        if (ctrl && ctrl->camera) {
            ctrl->camera->processMouse(xoffset, yoffset);
        }
    }
}

