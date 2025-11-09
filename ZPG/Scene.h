#pragma once
#include <vector>
#include "DrawableObject.h"
#include "Skybox.h"

class Scene {
public:
    Scene();
    ~Scene();

    void addObject(DrawableObject* object);
    void render();
    void setSkybox(Skybox* sb);

private:
    std::vector<DrawableObject*> objects;
    Skybox* skybox;
};
