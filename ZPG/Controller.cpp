#include "Controller.h"
#include "Scene.h"
#include "Application.h"  // Pro 'app->currentId'
#include "Translate.h"    // Pro klonování a dynamický cast
#include <stdio.h> 

// Globální ukazatel na aplikaci
extern Application* app;

// --- Definice statických promìnných ---
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

    // --- 1. VÝBÌR / PØEMÍSTÌNÍ (Levé tlaèítko) ---
    if (button == GLFW_MOUSE_BUTTON_LEFT) {

        // --- A) PØEMÍSTÌNÍ (Alt + Klik) ---
        if (mods == GLFW_MOD_ALT) {
            DrawableObject* selected = controller->activeScene->getSelectedObject();
            if (selected == nullptr) {
                printf("Nelze premistit: Zadny objekt neni vybran.\n");
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

                selected->setTranslation(worldPos); // Použijeme metodu, co jsi vytvoøil
                printf("Objekt ID %d premisten na [%f, %f, %f]\n", selected->getID(), worldPos.x, worldPos.y, worldPos.z);
            }
            else {
                printf("Nelze premistit na skybox.\n");
            }
        }
        // --- B) BÌŽNÝ VÝBÌR ---
        else {
            GLuint index = 0;
            glReadPixels(x, y_new, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
            controller->activeScene->selectObjectByID(index);
        }
    }

    // --- 2. KLONOVÁNÍ (Sázení) (Støední tlaèítko) ---
    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {

        DrawableObject* selected = controller->activeScene->getSelectedObject();
        if (selected == nullptr) {
            printf("Nelze klonovat: Zadny objekt neni vybran.\n");
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
                printf("Dosazen maximalni pocet objektu (255)!\n");
                app->currentId = 255;
                return;
            }

            printf("Klonovani objektu ID %d na pozici [%f, %f, %f] s novym ID %d\n",
                selected->getID(), worldPos.x, worldPos.y, worldPos.z, newID);

            // Vytvoøíme klon (zatím bez transformací)
            DrawableObject* clone = new DrawableObject(
                selected->getModel(),
                selected->getShaderProgram(),
                selected->getMaterial(),
                newID,
                selected->getTexture()
            );

            // --- ZMÌNA: Kopírování transformací ---
            // Projdeme všechny transformace pùvodního objektu
            for (const auto* t : selected->getTransformations()) {

                // Zkusíme, jestli to není Translate
                const Translate* trans = dynamic_cast<const Translate*>(t);

                if (trans != nullptr) {
                    // Je to Translate. Pøeskoèíme ho (nahradíme ho novým).
                    continue;
                }
                else {
                    // Je to Scale nebo Rotate, naklonujeme ho
                    clone->addTransformation(t->clone());
                }
            }
            // Nakonec pøidáme novou Translaci s pozicí kliknutí
            clone->addTransformation(new Translate(worldPos));
            // ------------------------------------

            controller->activeScene->addObject(clone);
        }
        else {
            printf("Nelze klonovat na skybox.\n");
        }
    }
}


// --- Callback pro pohyb myši (beze zmìny) ---
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
        float yoffset = (float)(ypos - lastY); // Inverzní ovládání

        lastX = xpos;
        lastY = ypos;

        controller->camera->processMouse(xoffset, yoffset);
    }
}