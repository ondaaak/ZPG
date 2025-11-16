#pragma once
#include <vector>
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include "Material.h"
#include "Texture.h"
#include <glm/glm.hpp> 
#include "Translate.h"

class DrawableObject {
public:
    DrawableObject(Model* model, ShaderProgram* shaderProgram,
        const Material& material, int id, Texture* texture = nullptr);

    ~DrawableObject();

    void addTransformation(Transformation* t) { transformations.push_back(t); }
    glm::mat4 getMatrix() const;
    void render() const;
    int getID() const { return id; }

    // --- PØIDANÉ METODY ---

    // Gettery pro klonování
    Model* getModel() const { return model; }
    ShaderProgram* getShaderProgram() const { return shaderProgram; }
    const Material& getMaterial() const { return material; }
    Texture* getTexture() const { return texture; }

    /**
     * Najde první transformaci typu Translate a nastaví její pozici.
     * Pokud žádná není, vytvoøí novou na zaèátku seznamu.
     */
    void setTranslation(const glm::vec3& newPosition);

private:
    Model* model;
    ShaderProgram* shaderProgram;
    std::vector<Transformation*> transformations;
    Material material;
    Texture* texture;
    int id;
};