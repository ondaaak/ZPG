#include "Controller.h"
#include "Scene.h"
#include <stdio.h>

bool Controller::rightMousePressed = false;
bool Controller::firstMouse = true;
double Controller::lastX = 0.0;
double Controller::lastY = 0.0;


Controller::Controller(Camera* camera, GLFWwindow* window, Scene* scene)
    : camera(camera), window(window), activeScene(scene)
{

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetWindowUserPointer(window, this);
}

void Controller::processInput(float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(GLFW_KEY_W, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(GLFW_KEY_S, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(GLFW_KEY_A, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(GLFW_KEY_D, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->processKeyboard(GLFW_KEY_SPACE, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera->processKeyboard(GLFW_KEY_LEFT_CONTROL, deltaTime);
}

void Controller::setActiveScene(Scene* scene) {
    this->activeScene = scene;
}


void Controller::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Controller* controller = static_cast<Controller*>(glfwGetWindowUserPointer(window));
    if (!controller) return;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    GLint y_new = height - (GLint)ypos;
    GLint x = (GLint)xpos;

    // --- 1. OTÁÈENÍ KAMERY (Pravé tlaèítko - Držet) ---
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rightMousePressed = true;
            firstMouse = true;
        }
        else if (action == GLFW_RELEASE) {
            rightMousePressed = false;
        }
    }

    // --- 2. OBJECT PICKING (Levé tlaèítko - Klik) ---
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        GLuint index = 0;
        glReadPixels(x, y_new, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

        // Místo printf zavoláme funkci scény
        // Ta se postará o výbìr (pokud index > 0) i zrušení výbìru (pokud index == 0)
        controller->activeScene->selectObjectByID(index);
    }

    // --- 3. UN-PROJECTION (Støední tlaèítko - Klik) ---
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        GLfloat depth = 0.0f;
        glReadPixels(x, y_new, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

        if (depth < 1.0f) {
            glm::mat4 view = controller->camera->getViewMatrix();
            glm::mat4 proj = controller->camera->getProjectionMatrix();
            glm::vec4 viewport = glm::vec4(0, 0, width, height);
            glm::vec3 screenPos = glm::vec3(xpos, y_new, depth);
            glm::vec3 worldPos = glm::unProject(screenPos, view, proj, viewport);

            printf("Svetove souradnice kliknuti: [%f, %f, %f]\n", worldPos.x, worldPos.y, worldPos.z);

            // Tady by pøišla logika pro vložení objektu
            // napø. controller->activeScene->vlozStrom(worldPos);
        }
        else {
            printf("Kliknuto na skybox, nelze vlozit objekt.\n");
        }
    }
}


void Controller::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    Controller* controller = static_cast<Controller*>(glfwGetWindowUserPointer(window));
    if (!controller) return;

    if (rightMousePressed) {

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = (float)(xpos - lastX);
        float yoffset = (float)(ypos - lastY);

        lastX = xpos;
        lastY = ypos;

        controller->camera->processMouse(xoffset, yoffset);
    }
}