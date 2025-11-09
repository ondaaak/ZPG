#include "Scene.h"

Scene::Scene() : skybox(nullptr) {}

Scene::~Scene() {

	for (auto obj : objects) {
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

void Scene::render() {
    for (auto obj : objects) {
        obj->render(); 
    }

    if (skybox) {
        skybox->draw();
    }
}
