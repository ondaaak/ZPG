#pragma once
#include <vector>
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include "Material.h"
#include "Texture.h"

class DrawableObject {
public:
    DrawableObject(Model* model, ShaderProgram* shaderProgram,
        const Material& material, int id, Texture* texture = nullptr);

    ~DrawableObject();

    void addTransformation(Transformation* t) { transformations.push_back(t); }
    void addTransformationToFront(Transformation* t) {
        transformations.insert(transformations.begin(), t);
    }

    glm::mat4 getMatrix() const;
    void render() const;
    int getID() const { return id; }

    void setTranslation(const glm::vec3& newPosition);

    DrawableObject* clone(int newID) const;


private:
    Model* model;
    ShaderProgram* shaderProgram;
    std::vector<Transformation*> transformations;
    Material material;
    Texture* texture;
    int id;
};