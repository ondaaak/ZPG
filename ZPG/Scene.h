#pragma once
#include <vector>
#include "DrawableObject.h"

class Scene {
public:
    Scene();
    ~Scene();

    void addObject(DrawableObject* object);
    void render();

private:
    std::vector<DrawableObject*> objects;
};
