#pragma once
#include <vector>
#include <algorithm> 
#include "DrawableObject.h"
#include "Skybox.h"
#include "Transformation.h"
#include "PathTransform.h"

class Scene {
public:
    Scene();
    ~Scene();

    void addObject(DrawableObject* object);
    void render();
    void setSkybox(Skybox* sb);

    void selectObjectByID(int id);
    void deleteSelectedObject();
    DrawableObject* getSelectedObject() const;
    void resetObjectAnimation(DrawableObject* obj);

    DrawableObject* findObjectByID(int id) const;

private:
    std::vector<DrawableObject*> objects;
    Skybox* skybox;
    DrawableObject* selectedObject;
};