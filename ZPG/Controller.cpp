#include "Controller.h"
#include "Scene.h"
#include "Application.h"  
#include "Translate.h"
#include "Scale.h"
#include "BezierTransform.h"
#include <stdio.h> 
#include <iostream>

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

    // Pøipravíme si zdroje pro vizuální znaèky bodù
    pointMarkerModel = new Model(sphere, sizeof(sphere) / sizeof(float) / 6, 1);
    markerShader = new ShaderProgram("main_vertex_shader.glsl", "phong_simple.glsl");
    if (camera) {
        camera->addObserver(markerShader);
    }
}

void Controller::setActiveScene(Scene* scene) {
    this->activeScene = scene;
    bezierObject = nullptr; // Resetujeme ukazatel

    // ZJEDNODUŠENÍ: Pokud jsme ve scénì 6 a má nìjaké objekty,
    // pøedpokládáme, že ten první je naše formule.
    if (activeScene && activeScene == app->getScene6()) {
        const auto& objects = activeScene->getObjects();
        if (!objects.empty()) {
            bezierObject = objects[0]; // Vezmeme první objekt
            std::cout << "Formula object assigned for Bezier curve." << std::endl;
        }
    }
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

    // --- NOVÁ LOGIKA PRO SCÉNU 6 ---
    if (button == GLFW_MOUSE_BUTTON_LEFT && controller->activeScene == app->getScene6())
    {
        if (controller->bezierObject == nullptr) return;

        // Ray-Casting pro nalezení bodu na podlaze
        glm::mat4 view = controller->camera->getViewMatrix();
        glm::mat4 proj = controller->camera->getProjectionMatrix();
        glm::vec4 viewport = glm::vec4(0, 0, width, height);

        glm::vec3 screenPosStart = glm::vec3(xpos, y_new, 0.0f); // Bod na blízké rovinì
        glm::vec3 screenPosEnd = glm::vec3(xpos, y_new, 1.0f);   // Bod na vzdálené rovinì

        glm::vec3 worldPosStart = glm::unProject(screenPosStart, view, proj, viewport);
        glm::vec3 worldPosEnd = glm::unProject(screenPosEnd, view, proj, viewport);

        glm::vec3 ray_dir = glm::normalize(worldPosEnd - worldPosStart);
        glm::vec3 ray_origin = controller->camera->getCameraPosition();

        glm::vec3 plane_normal = glm::vec3(0.0f, 1.0f, 0.0f);
        float denom = glm::dot(plane_normal, ray_dir);

        if (std::abs(denom) > 1e-6) {
            float t = glm::dot(glm::vec3(0, 0, 0) - ray_origin, plane_normal) / denom;
            if (t >= 0) {
                glm::vec3 intersectionPoint = ray_origin + t * ray_dir;
                controller->controlPoints.push_back(intersectionPoint);

                // Vizuální znaèka
                Material redMarker; redMarker.ambient = glm::vec3(1, 0, 0); redMarker.diffuse = glm::vec3(1, 0, 0);
                DrawableObject* marker = new DrawableObject(controller->pointMarkerModel, controller->markerShader, redMarker, 0, nullptr);
                marker->addTransformation(new Translate(intersectionPoint));
                marker->addTransformation(new Scale(glm::vec3(0.1f)));
                controller->activeScene->addObject(marker);

                // Aktualizace køivky
                if (controller->controlPoints.size() >= 4 && (controller->controlPoints.size() - 4) % 3 == 0) {
                    controller->bezierObject->removeTransformation(controller->bezierObject->getTransformations()[0]);
                    BezierTransform* newBezier = new BezierTransform(controller->controlPoints, 0.3f, true);
                    controller->bezierObject->addTransformationToFront(newBezier);
                }
            }
        }
        return; // Ukonèíme funkci, aby se nespustil další kód pro výbìr objektù
    }
    // --- KONEC NOVÉ LOGIKY ---


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