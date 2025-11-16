#include "DrawableObject.h"


DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram,
    const Material& material, int id, Texture* texture)
    : model(model), shaderProgram(shaderProgram), material(material),
    texture(texture), id(id)
{
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

// --- PØIDANÁ METODA ---
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

    // Pokud jsme žádný Translate nenašli (objekt má tøeba jen Scale/Rotate),
    // vytvoøíme nový a vložíme ho na ZAÈÁTEK seznamu.
    // (Je dùležité, aby byla translace až po rotaci/škálování,
    // ale pro jednoduchost ji dáme na zaèátek).
    // Pro správné poøadí (TRS) bychom ji vložili na konec:
    // transformations.push_back(new Translate(newPosition));

    // Pro poøadí TRS (Translate * Rotate * Scale) ji musíme vložit na zaèátek
    // POZOR: getMatrix() násobí v poøadí, v jakém jsou v seznamu.
    // Aby byla translace poslední (mat = ... * T), musí být na konci seznamu.
    // Ale tvoje objekty (napø. koèka) mají poøadí S * R * T.

    // Dle tvého kódu (napø. catObject) pøidáváš transformace v poøadí S, R, T.
    // Proto pøidáme novou Translaci na konec.
    transformations.push_back(new Translate(newPosition));
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