#include "DrawableObject.h"
#include "Material.h" // P�id�no

// ZM�NA: Konstruktor inicializuje materi�l
DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram, const Material& material)
    : model(model), shaderProgram(shaderProgram), material(material) {
}

// ZM�NA: Destruktor je implementov�n
DrawableObject::~DrawableObject() {
    //for (auto t : transformations) delete t;
}

glm::mat4 DrawableObject::getMatrix() const {
    glm::mat4 mat(1.0f);
    for (const auto& t : transformations) {
        // Pou��v�me tvou p�vodn� logiku n�soben�
        mat = mat * t->getMatrix();
    }
    return mat;
}

void DrawableObject::render() const {
    shaderProgram->setShaderProgram();
    shaderProgram->SetUniform("modelMatrix", getMatrix());

    // ZM�NA: P�id�no nastaven� uniform� pro materi�l
    shaderProgram->SetUniform("material.ambient", material.ambient);
    shaderProgram->SetUniform("material.diffuse", material.diffuse);
    shaderProgram->SetUniform("material.specular", material.specular);
    shaderProgram->SetUniform("material.shininess", material.shininess);

    model->draw();
}

// ZM�NA: P�id�na implementace setMaterial
void DrawableObject::setMaterial(const Material& newMaterial) {
    material = newMaterial;
}