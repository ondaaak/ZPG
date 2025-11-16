#include "DrawableObject.h"
#include "Translate.h" // Potøebné pro setTranslation

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram,
    const Material& material, int id, Texture* texture)
    : model(model), shaderProgram(shaderProgram), material(material),
    texture(texture), id(id)
{
}

DrawableObject::~DrawableObject() {
    for (auto t : transformations) delete t;
}

glm::mat4 DrawableObject::getMatrix() const {
    glm::mat4 mat(1.0f);
    // Matice se násobí v poøadí T * S * R
    // (Vertex je nejdøív rotován, pak škálován, pak pøemístìn)
    for (const auto& t : transformations) {
        mat = mat * t->getMatrix();
    }
    return mat;
}

void DrawableObject::setTranslation(const glm::vec3& newPosition) {
    // Projdeme všechny transformace
    for (auto* t : transformations) {
        // Pokusíme se pøetypovat na Translate
        Translate* trans = dynamic_cast<Translate*>(t);
        if (trans != nullptr) {
            // Našli jsme první Translate, zmìníme ho a konèíme
            trans->setOffset(newPosition);
            return;
        }
    }

    // Pokud jsme žádný Translate nenašli, vytvoøíme nový a
    // vložíme ho na ZAÈÁTEK seznamu, aby bylo zachováno poøadí T*S*R.
    transformations.insert(transformations.begin(), new Translate(newPosition));
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
        if (texture != nullptr) {
            shaderProgram->SetUniform("useTexture", 1);
            texture->bind(GL_TEXTURE0);
        }
        else {
            shaderProgram->SetUniform("useTexture", 0);
        }
    }

    model->draw();
}