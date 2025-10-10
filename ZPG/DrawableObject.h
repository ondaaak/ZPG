#pragma once
#include <vector>
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"

class DrawableObject {
public:
    DrawableObject(Model* model, ShaderProgram* shaderProgram);
    ~DrawableObject();

    void addTransformation(Transformation* t) { transformations.push_back(t); }
    glm::mat4 getMatrix() const;

    void render() const;

private:
    Model* model;
    ShaderProgram* shaderProgram;
    std::vector<Transformation*> transformations;
};