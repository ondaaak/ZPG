#include "Scene.h"

Scene::Scene() {}

Scene::~Scene() {

	for (auto obj : objects) {
		delete obj;
	}
	objects.clear();

}

void Scene::addObject(DrawableObject* object) {
    objects.push_back(object);
}

void Scene::render() {
    for (auto obj : objects) {
        obj->render(); 
    }
}
