#include "Scene.h"
#include <stdio.h> // Pro printf

Scene::Scene()
    : skybox(nullptr), selectedObject(nullptr) // Inicializujeme selectedObject
{
}

Scene::~Scene() {
    // Smažeme všechny objekty, které scéna vlastní
    for (auto* obj : objects) {
        delete obj;
    }
    objects.clear();

    // --- OPRAVA CHYBY è. 1 ---
    // Skybox je vlastnìn a mazán v Application.cpp
    // Odebereme tento øádek, abychom zabránili dvojitému smazání.
    /*
    if (skybox) {
        delete skybox;
    }
    */
    // -------------------------
}

void Scene::addObject(DrawableObject* object) {
    objects.push_back(object);
}

void Scene::setSkybox(Skybox* sb) {
    skybox = sb;
}

// --- ZDE ZAÈÍNÁ NOVÁ LOGIKA ---

/**
 * Najde objekt podle jeho ID a nastaví ho jako 'selectedObject'.
 * Pokud je ID 0 (pozadí), zruší výbìr.
 */
void Scene::selectObjectByID(int id) {
    if (id == 0) {
        if (selectedObject != nullptr) {
            printf("Vyber zrusen.\n");
        }
        selectedObject = nullptr;
        return;
    }

    for (DrawableObject* obj : objects) {
        if (obj->getID() == id) {
            selectedObject = obj;
            printf("Objekt vybran! ID: %d\n", id);
            return;
        }
    }

    printf("Chyba: Objekt s ID %d nenalezen.\n", id);
    selectedObject = nullptr;
}

/**
 * Smaže aktuálnì vybraný objekt ze scény.
 */
void Scene::deleteSelectedObject() {
    if (selectedObject == nullptr) {
        printf("Zadny objekt neni vybran k smazani.\n");
        return;
    }

    auto it = std::find(objects.begin(), objects.end(), selectedObject);

    if (it != objects.end()) {
        printf("Mazani objektu s ID: %d\n", selectedObject->getID());

        objects.erase(it);
        delete selectedObject; // Tímto se zavolá DrawableObject::~DrawableObject()
        selectedObject = nullptr;
    }
}

/**
 * Vrátí ukazatel na aktuálnì vybraný objekt.
 */
DrawableObject* Scene::getSelectedObject() const {
    return selectedObject;
}


// --- ZDE JE TVOJE STÁVAJÍCÍ FUNKCE RENDER ---
void Scene::render() {
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);

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