#include "DrawableObject.h"
#include "Translate.h" 

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram, 
                               const Material& material, int id, Texture* texture)
    : model(model), shaderProgram(shaderProgram), material(material), 
      texture(texture), id(id)
{
}

DrawableObject::~DrawableObject() {
    //for (auto t : transformations) delete t;
}

DrawableObject* DrawableObject::clone(int newID) const
{
    DrawableObject* newClone = new DrawableObject(
        this->model,
        this->shaderProgram,
        this->material,
        newID,
        this->texture
    );

    for (const auto* t : this->transformations) {
        newClone->addTransformation(t->clone());
    }

    return newClone;
}

glm::mat4 DrawableObject::getMatrix() const {
    glm::mat4 mat(1.0f);
    for (const auto& t : transformations) {
        mat = mat * t->getMatrix(); 
    }
    return mat;
}

void DrawableObject::setTranslation(const glm::vec3& newPosition) {
    for (auto* t : transformations) {
        Translate* trans = dynamic_cast<Translate*>(t);
        if (trans != nullptr) {
            trans->setOffset(newPosition);
            return;
        }
    }
    transformations.insert(transformations.begin(), new Translate(newPosition));
}

void DrawableObject::removeTransformation(Transformation* t) {
    auto it = std::find(transformations.begin(), transformations.end(), t);
    if (it != transformations.end()) {
        delete* it; 
        transformations.erase(it);
    }
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