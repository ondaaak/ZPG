#pragma once
#include <vector>
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include "Material.h"
#include "Texture.h"
#include <glm/glm.hpp> // Pøidáno pro glm::mat4

class DrawableObject {
public:
    /**
     * Konstruktor pro objekt.
     * @param id Unikátní ID (1-255) pro stencil buffer.
     * @param texture Ukazatel na texturu, mùže být nullptr.
     */
    DrawableObject(Model* model, ShaderProgram* shaderProgram,
        const Material& material, int id, Texture* texture = nullptr);

    ~DrawableObject();

    void addTransformation(Transformation* t) { transformations.push_back(t); }
    glm::mat4 getMatrix() const;

    void render() const;

    // Getter pro ID, které budeme zapisovat do stencil bufferu
    int getID() const { return id; }

private:
    Model* model;
    ShaderProgram* shaderProgram;
    std::vector<Transformation*> transformations;
    Material material;
    Texture* texture; // Mùže být nullptr

    // ID objektu
    int id;
};