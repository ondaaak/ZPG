#pragma once
#include <vector>
#include <algorithm> // Potøebujeme pro std::find
#include "DrawableObject.h"
#include "Skybox.h"

class Scene {
public:
    Scene();
    ~Scene();

    void addObject(DrawableObject* object);
    void render();
    void setSkybox(Skybox* sb);

    // --- NOVÉ METODY ---

    /**
     * Najde objekt podle jeho ID a nastaví ho jako 'selectedObject'.
     * Pokud je ID 0 (pozadí), zruší výbìr.
     */
    void selectObjectByID(int id);

    /**
     * Smaže aktuálnì vybraný objekt ze scény.
     */
    void deleteSelectedObject();

    /**
     * Vrátí ukazatel na aktuálnì vybraný objekt.
     */
    DrawableObject* getSelectedObject() const;

private:
    std::vector<DrawableObject*> objects;
    Skybox* skybox;

    // --- NOVÁ PROMÌNNÁ ---
    DrawableObject* selectedObject; // Ukazatel na vybraný objekt
};