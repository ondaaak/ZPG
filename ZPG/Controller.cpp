#include "Controller.h"
#include "Scene.h"
#include "Application.h"  
#include "Translate.h"    
#include <stdio.h> 

extern Application* app;

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

void Controller::setActiveScene(Scene* scene) {
    this->activeScene = scene;
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

void Controller::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Controller* controller = static_cast<Controller*>(glfwGetWindowUserPointer(window));
    if (!controller) return;

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rightMousePressed = true;
            firstMouse = true;
        }
        else if (action == GLFW_RELEASE) {
            rightMousePressed = false;
        }
    }

    if (action != GLFW_PRESS) return;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    GLint y_new = height - (GLint)ypos;
    GLint x = (GLint)xpos;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {

        if (controller->activeScene == app->getScene5() && mods == 0) {
            GLuint index = 0;
            glReadPixels(x, y_new, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

            if (index > 0) {
                
                DrawableObject* hitObject = controller->activeScene->findObjectByID(index);

                if (hitObject) {
                    controller->activeScene->resetObjectAnimation(hitObject);
                    printf("Mole hit! Object ID %d reset.\n", index);
                }
            }
            return;  
        }

        

        if (mods == GLFW_MOD_ALT) {
            DrawableObject* selected = controller->activeScene->getSelectedObject();
            if (selected == nullptr) {
                printf("No object selected. Can't move.\n");
                return;
            }
            GLfloat depth = 0.0f;
            glReadPixels(x, y_new, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
            if (depth < 1.0f) {
                glm::mat4 view = controller->camera->getViewMatrix();
                glm::mat4 proj = controller->camera->getProjectionMatrix();
                glm::vec4 viewport = glm::vec4(0, 0, width, height);
                glm::vec3 screenPos = glm::vec3(xpos, y_new, depth);
                glm::vec3 worldPos = glm::unProject(screenPos, view, proj, viewport);
                selected->setTranslation(worldPos);
                printf("Object ID %d moved to [%f, %f, %f]\n", selected->getID(), worldPos.x, worldPos.y, worldPos.z);
            }
            else {
                printf("Can't move to skybox.\n");
            }
        }
        else {
            GLuint index = 0;
            glReadPixels(x, y_new, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
            controller->activeScene->selectObjectByID(index);
        }
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {

        DrawableObject* selected = controller->activeScene->getSelectedObject();
        if (selected == nullptr) {
            printf("No object selected. Can't clone.\n");
            return;
        }

        GLfloat depth = 0.0f;
        glReadPixels(x, y_new, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

        if (depth < 1.0f) {
            glm::mat4 view = controller->camera->getViewMatrix();
            glm::mat4 proj = controller->camera->getProjectionMatrix();
            glm::vec4 viewport = glm::vec4(0, 0, width, height);
            glm::vec3 screenPos = glm::vec3(xpos, y_new, depth);
            glm::vec3 worldPos = glm::unProject(screenPos, view, proj, viewport);

            int newID = app->currentId++;
            if (newID > 255) {
                printf("Maximum number of objects reached (255)\n");
                app->currentId = 255;
                return;
            }

            printf("Cloning object ID %d to [%f, %f, %f] with new ID %d\n",
                selected->getID(), worldPos.x, worldPos.y, worldPos.z, newID);

            DrawableObject* clone = selected->clone(newID);

            clone->setTranslation(worldPos);

            controller->activeScene->addObject(clone);

        }
        else {
            printf("Can't put clone on skybox.\n");
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