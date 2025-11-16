#include "Controller.h"
#include "Scene.h"
#include "Application.h"  // <-- PØIDAT INCLUDE (pro pøístup k 'app->currentId')
#include "Translate.h"    // <-- PØIDAT INCLUDE (pro klonování)
#include <stdio.h> 

// Globální ukazatel na aplikaci (z Application.cpp)
extern Application* app;

// --- Definice statických promìnných ---
bool Controller::rightMousePressed = false;
bool Controller::firstMouse = true;
double Controller::lastX = 0.0;
double Controller::lastY = 0.0;

void Controller::setActiveScene(Scene* scene) {
    this->activeScene = scene;
}

Controller::Controller(Camera* camera, GLFWwindow* window, Scene* scene)
    : camera(camera), window(window), activeScene(scene)
{
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetWindowUserPointer(window, this);
}

void Controller::processInput(float deltaTime)
{
    // ... (kód pro klávesnici W, S, A, D ...)
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


// --- Callback pro kliknutí myši ---
void Controller::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Controller* controller = static_cast<Controller*>(glfwGetWindowUserPointer(window));
    if (!controller) return;

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

    // Zpracováváme pouze STISK tlaèítka
    if (action != GLFW_PRESS) return;


    // Získání pozice a matic pro unProject
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    GLint y_new = height - (GLint)ypos;
    GLint x = (GLint)xpos;

    // --- 2. VÝBÌR / PØEMÍSTÌNÍ (Levé tlaèítko - Klik) ---
    if (button == GLFW_MOUSE_BUTTON_LEFT) {

        // --- A) PØEMÍSTÌNÍ ---
        if (mods == GLFW_MOD_ALT) {
            DrawableObject* selected = controller->activeScene->getSelectedObject();
            if (selected == nullptr) {
                printf("Nelze premistit: Zadny objekt neni vybran.\n");
                return;
            }

            // Èteme hloubku pro unProject
            GLfloat depth = 0.0f;
            glReadPixels(x, y_new, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

            if (depth < 1.0f) {
                glm::mat4 view = controller->camera->getViewMatrix();
                glm::mat4 proj = controller->camera->getProjectionMatrix();
                glm::vec4 viewport = glm::vec4(0, 0, width, height);
                glm::vec3 screenPos = glm::vec3(xpos, y_new, depth);
                glm::vec3 worldPos = glm::unProject(screenPos, view, proj, viewport);

                // Zavoláme naši novou metodu pro pøemístìní
                selected->setTranslation(worldPos);
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

    // --- 3. KLONOVÁNÍ (Sázení) (Støední tlaèítko - Klik) ---
    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {

        DrawableObject* selected = controller->activeScene->getSelectedObject();
        if (selected == nullptr) {
            printf("Nelze klonovat: Zadny objekt neni vybran.\n");
            return;
        }

        // Èteme hloubku pro unProject
        GLfloat depth = 0.0f;
        glReadPixels(x, y_new, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

        if (depth < 1.0f) {
            glm::mat4 view = controller->camera->getViewMatrix();
            glm::mat4 proj = controller->camera->getProjectionMatrix();
            glm::vec4 viewport = glm::vec4(0, 0, width, height);
            glm::vec3 screenPos = glm::vec3(xpos, y_new, depth);
            glm::vec3 worldPos = glm::unProject(screenPos, view, proj, viewport);

            // Získáme nové ID z aplikace
            int newID = app->currentId++;
            if (newID > 255) {
                printf("Dosazen maximalni pocet objektu (255)!\n");
                app->currentId = 255; // Reset
                return;
            }

            printf("Klonovani objektu ID %d na pozici [%f, %f, %f] s novym ID %d\n",
                selected->getID(), worldPos.x, worldPos.y, worldPos.z, newID);

            // Vytvoøíme klon
            DrawableObject* clone = new DrawableObject(
                selected->getModel(),
                selected->getShaderProgram(),
                selected->getMaterial(),
                newID,
                selected->getTexture()
            );

            // Nastavíme mu pouze novou translaci (bez rotace/škálování)
            // Pokud bys chtìl klonovat i ty, musel bys zkopírovat celý seznam transformací
            clone->addTransformation(new Translate(worldPos));

            // Pøidáme ho do scény
            controller->activeScene->addObject(clone);

        }
        else {
            printf("Nelze klonovat na skybox.\n");
        }
    }
}


// --- Callback pro pohyb myši ---
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