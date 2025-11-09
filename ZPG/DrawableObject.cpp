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

    if (shaderProgram->HasUniform("useTexture"))
    {
        if (texture) {
            shaderProgram->SetUniform("useTexture", 1);
            texture->bind(GL_TEXTURE0);
        }
        else {
            shaderProgram->SetUniform("useTexture", 0);
        }
    }

    model->draw();
}