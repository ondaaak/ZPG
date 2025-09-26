#include "Scene.h"

Scene::Scene(ShaderProgram* shaderProgram)
    : shaderProgram(shaderProgram)
{}

Scene::~Scene() {}

void Scene::addObject(DrawableObject* object) {
    objects.push_back(object);
}

void Scene::render() {
    shaderProgram->setShaderProgram();
    for (auto obj : objects) {
        obj->draw();
    }
}
