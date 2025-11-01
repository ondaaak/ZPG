#include "DrawableObject.h"
#include "Material.h" // Pøidáno

// ZMÌNA: Konstruktor inicializuje materiál
DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram, const Material& material)
    : model(model), shaderProgram(shaderProgram), material(material) {
}

// ZMÌNA: Destruktor je implementován
DrawableObject::~DrawableObject() {
    //for (auto t : transformations) delete t;
}

glm::mat4 DrawableObject::getMatrix() const {
    glm::mat4 mat(1.0f);
    for (const auto& t : transformations) {
        // Používáme tvou pùvodní logiku násobení
        mat = mat * t->getMatrix();
    }
    return mat;
}

void DrawableObject::render() const {
    shaderProgram->setShaderProgram();
    shaderProgram->SetUniform("modelMatrix", getMatrix());

    // ZMÌNA: Pøidáno nastavení uniformù pro materiál
    shaderProgram->SetUniform("material.ambient", material.ambient);
    shaderProgram->SetUniform("material.diffuse", material.diffuse);
    shaderProgram->SetUniform("material.specular", material.specular);
    shaderProgram->SetUniform("material.shininess", material.shininess);

    model->draw();
}

// ZMÌNA: Pøidána implementace setMaterial
void DrawableObject::setMaterial(const Material& newMaterial) {
    material = newMaterial;
}