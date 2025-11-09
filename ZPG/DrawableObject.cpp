#include "DrawableObject.h"

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram, const Material& material, Texture* texture)
    : model(model), shaderProgram(shaderProgram), material(material), texture(texture) {
}


DrawableObject::~DrawableObject() {
    //for (auto t : transformations) delete t;
}

glm::mat4 DrawableObject::getMatrix() const {
    glm::mat4 mat(1.0f);
    for (const auto& t : transformations) {
        mat = mat * t->getMatrix();
    }
    return mat;
}

void DrawableObject::render() const {
    shaderProgram->setShaderProgram();
    shaderProgram->SetUniform("modelMatrix", getMatrix());

    if (shaderProgram->HasUniform("material.ambient")) {
        shaderProgram->SetUniform("material.ambient", material.ambient);
        shaderProgram->SetUniform("material.diffuse", material.diffuse);
        shaderProgram->SetUniform("material.specular", material.specular);
        shaderProgram->SetUniform("material.shininess", material.shininess);
    }

    // --- ZDE JE FINÁLNÍ OPRAVA ---
    // Zkusíme nastavit texturu, POUZE POKUD shader má uniform "useTexture"
    if (shaderProgram->HasUniform("useTexture"))
    {
        if (texture) {
            // Shader to podporuje A my máme texturu
            shaderProgram->SetUniform("useTexture", 1);
            texture->bind(GL_TEXTURE0);
        }
        else {
            // Shader to podporuje, ale my texturu nemáme
            shaderProgram->SetUniform("useTexture", 0);
        }
    }
    // -----------------------------
    // Pokud shader "useTexture" nemá (jako napø. spheresProgram), 
    // celı tento blok se pøeskoèí a nic se nestane.

    model->draw();
}