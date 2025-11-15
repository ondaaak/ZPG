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
