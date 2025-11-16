#pragma once
#include <vector>
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include "Material.h"
#include "Texture.h"
#include "Translate.h"
#include <glm/glm.hpp> 

class DrawableObject {
public:
    DrawableObject(Model* model, ShaderProgram* shaderProgram,
        const Material& material, int id, Texture* texture = nullptr);

    ~DrawableObject();

    void addTransformation(Transformation* t) { transformations.push_back(t); }
    glm::mat4 getMatrix() const;
    void render() const;
    int getID() const { return id; }

    // Gettery pro klonování
    Model* getModel() const { return model; }
    ShaderProgram* getShaderProgram() const { return shaderProgram; }
    const Material& getMaterial() const { return material; }
    Texture* getTexture() const { return texture; }

    /**
     * Najde první transformaci typu Translate a nastaví její pozici.
     */
    void setTranslation(const glm::vec3& newPosition);

    // --- PØIDEJ TENTO ØÁDEK ---
    const std::vector<Transformation*>& getTransformations() const { return transformations; }

private:
    Model* model;
    ShaderProgram* shaderProgram;
    std::vector<Transformation*> transformations;
    Material material;
    Texture* texture;
    int id;
};