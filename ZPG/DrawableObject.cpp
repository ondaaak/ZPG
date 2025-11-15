#include "DrawableObject.h"


DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram,
    const Material& material, int id, Texture* texture)
    : model(model), shaderProgram(shaderProgram), material(material),
    texture(texture), id(id)
{
    // Konstruktor je v poøádku
}

DrawableObject::~DrawableObject() {
    // Odkomentoval jsem ti mazání transformací, pokud je vlastníš.
    // Pokud je sdílíš, nech to zakomentované.
    // for (auto t : transformations) delete t;
}

glm::mat4 DrawableObject::getMatrix() const {
    glm::mat4 mat(1.0f);
    for (const auto& t : transformations) {
        mat = mat * t->getMatrix();
    }
    return mat;
}

void DrawableObject::render() const {
    // Nastavíme shader pro tento objekt
    shaderProgram->setShaderProgram();

    // Nastavíme uniformy pro transformaci a materiál
    shaderProgram->SetUniform("modelMatrix", getMatrix());

    if (shaderProgram->HasUniform("material.ambient")) {
        shaderProgram->SetUniform("material.ambient", material.ambient);
        shaderProgram->SetUniform("material.diffuse", material.diffuse);
        shaderProgram->SetUniform("material.specular", material.specular);
        shaderProgram->SetUniform("material.shininess", material.shininess);
    }

    // Tady je ta klíèová logika pro textury
    if (shaderProgram->HasUniform("useTexture"))
    {
        if (texture != nullptr) {
            // Objekt MÁ texturu
            shaderProgram->SetUniform("useTexture", 1);
            texture->bind(GL_TEXTURE0); // Bìnì se binduje na jednotku 0
        }
        else {
            // Objekt NEMÁ texturu
            shaderProgram->SetUniform("useTexture", 0);
        }
    }

    // Vykreslíme model
    model->draw();
}