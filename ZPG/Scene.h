#pragma once
#include <vector>
#include "DrawableObject.h"
#include "ShaderProgram.h"

class Scene {
public:
    Scene(ShaderProgram* shaderProgram);
    ~Scene();

    void addObject(DrawableObject* object);
    void render();
    // pøípadnì update(), handleInput() atd.

private:
    std::vector<DrawableObject*> objects;
    ShaderProgram* shaderProgram;
};
