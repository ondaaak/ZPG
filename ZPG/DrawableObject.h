#pragma once
#include <vector>
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include "Material.h" // Pøidáno

class DrawableObject {
public:
    // ZMÌNA: Konstruktor teï bere materiál
    DrawableObject(Model* model, ShaderProgram* shaderProgram, const Material& material);
    ~DrawableObject(); // Odkomentováno, aby se zabránilo memory leaku

    void addTransformation(Transformation* t) { transformations.push_back(t); }
    glm::mat4 getMatrix() const;

    void render() const;
    void setMaterial(const Material& newMaterial); // Pøidáno

private:
    Model* model;
    ShaderProgram* shaderProgram;
    std::vector<Transformation*> transformations;
    Material material; // Pøidáno
};