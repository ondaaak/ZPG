#pragma once
#include <vector>
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include "Material.h" // P�id�no

class DrawableObject {
public:
    // ZM�NA: Konstruktor te� bere materi�l
    DrawableObject(Model* model, ShaderProgram* shaderProgram, const Material& material);
    ~DrawableObject(); // Odkomentov�no, aby se zabr�nilo memory leaku

    void addTransformation(Transformation* t) { transformations.push_back(t); }
    glm::mat4 getMatrix() const;

    void render() const;
    void setMaterial(const Material& newMaterial); // P�id�no

private:
    Model* model;
    ShaderProgram* shaderProgram;
    std::vector<Transformation*> transformations;
    Material material; // P�id�no
};