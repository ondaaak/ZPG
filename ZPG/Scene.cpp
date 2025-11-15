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
    // --- ZAÈÁTEK ÚPRAVY DLE ZADÁNÍ ---

    // 1. Povolíme Stencil Test a nastavíme operace
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Co dìlat, kdy test projde/neprojde
    glStencilMask(0xFF); // Povolíme zápis do stencil bufferu

    // Mùeš chtít vyèistit stencil buffer na zaèátku kadého framu
    // (obvykle se to dìlá v hlavní smyèce v Application.cpp spolu s GL_COLOR_BUFFER_BIT)
    // glClear(GL_STENCIL_BUFFER_BIT); 

    // ---

    // 2. Vykreslíme všechny objekty s jejich ID
    for (DrawableObject* obj : objects) {
        if (obj == nullptr) continue;

        // Nastavíme, e test projde VDY, a zapíšeme ID objektu
        glStencilFunc(GL_ALWAYS, obj->getID(), 0xFF);

        // Vykreslíme objekt (tato funkce by mìla nastavit shader, uniformy a volat glDrawArrays)
        obj->render();
    }

    // 3. Vypneme Stencil Test, aby neovlivòoval zbytek (napø. skybox)
    glDisable(GL_STENCIL_TEST);

    // --- KONEC ÚPRAVY ---

    // Vykreslíme skybox (ten by nemìl zapisovat do stencil bufferu)
    if (skybox) {
        skybox->draw();
    }
}
