#include "Scene.h"
#include <stdio.h>

Scene::Scene()
    : skybox(nullptr), selectedObject(nullptr)
{
}

Scene::~Scene() {

    for (auto* obj : objects) {
        delete obj;
    }
    objects.clear();
}

void Scene::addObject(DrawableObject* object) {
    objects.push_back(object);
}

void Scene::setSkybox(Skybox* sb) {
    skybox = sb;
}

void Scene::selectObjectByID(int id) {
    if (id == 0) {
        if (selectedObject != nullptr) {
            printf("Deselected object.\n");
        }
        selectedObject = nullptr;
        return;
    }

    for (DrawableObject* obj : objects) {
        if (obj->getID() == id) {
            selectedObject = obj;
            printf("Object ID %d selected.\n", id);

            return;
        }
    }

    printf("Object ID %d not found.\n", id);
    selectedObject = nullptr;
}

void Scene::deleteSelectedObject() {
    if (selectedObject == nullptr) {
        printf("No object selected.\n");
        return;
    }

    auto it = std::find(objects.begin(), objects.end(), selectedObject);

    if (it != objects.end()) {
        printf("Deleting object ID: %d\n", selectedObject->getID());

        objects.erase(it);
        delete selectedObject; 
        selectedObject = nullptr;
    }
}

DrawableObject* Scene::getSelectedObject() const {
    return selectedObject;
}

DrawableObject* Scene::findObjectByID(int id) const {
    for (DrawableObject* obj : objects) {
        if (obj->getID() == id) {
            return obj;
        }
    }
    return nullptr;
}

void Scene::resetObjectAnimation(DrawableObject* obj) {
    if (!obj) return;

    for (Transformation* transform : obj->getTransformations()) {
        PathTransform* pathTransform = dynamic_cast<PathTransform*>(transform);
        if (pathTransform) {
            pathTransform->reset();
            printf("Animation reset for object ID %d\n", obj->getID());
            return;
        }
    }

    printf("No PathTransform found for object ID %d\n", obj->getID());
}

void Scene::render() {
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    for (DrawableObject* obj : objects) {
        if (obj == nullptr) continue;
        glStencilFunc(GL_ALWAYS, obj->getID(), 0xFF);
        obj->render();
    }

    glDisable(GL_STENCIL_TEST);

    if (skybox) {
        skybox->draw();
    }
}