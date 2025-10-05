#pragma once
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"

class DrawableObject {
public:
    DrawableObject(Model* model, ShaderProgram* shaderProgram);
    ~DrawableObject();

    void render() const;

    glm::mat4 getModelMatrix() const;
    Transformation& getTransformation();
    const Transformation& getTransformation() const;

private:
    Model* model;
    ShaderProgram* shaderProgram;
    Transformation transformation;
};
