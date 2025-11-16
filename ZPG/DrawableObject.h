#pragma once
#include <vector>
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include "Material.h"
#include "Texture.h"
#include <glm/glm.hpp> 

class DrawableObject {
public:
    DrawableObject(Model* model, ShaderProgram* shaderProgram,
        const Material& material, int id, Texture* texture = nullptr);

    ~DrawableObject();

    void addTransformation(Transformation* t) { transformations.push_back(t); }

    // --- PØIDÁNO ---
    /**
     * Pøidá transformaci na zaèátek seznamu (pro zajištìní správného poøadí T*R*S).
     */
    void addTransformationToFront(Transformation* t) {
        transformations.insert(transformations.begin(), t);
    }
    // -------------

    glm::mat4 getMatrix() const;
    void render() const;
    int getID() const { return id; }

    // Gettery pro klonování
    Model* getModel() const { return model; }
    ShaderProgram* getShaderProgram() const { return shaderProgram; }
    const Material& getMaterial() const { return material; }
    Texture* getTexture() const { return texture; }
    const std::vector<Transformation*>& getTransformations() const { return transformations; }

    void setTranslation(const glm::vec3& newPosition);

private:
    Model* model;
    ShaderProgram* shaderProgram;
    std::vector<Transformation*> transformations;
    Material material;
    Texture* texture;
    int id;
};