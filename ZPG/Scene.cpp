#include "Scene.h"

Scene::Scene() {}

Scene::~Scene() {}

void Scene::addObject(DrawableObject* object) {
    objects.push_back(object);
}

void Scene::render() {
    for (auto obj : objects) {
        obj->render(); 
    }
}
