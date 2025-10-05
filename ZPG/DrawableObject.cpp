#include "DrawableObject.h"
#include <glm/gtc/type_ptr.hpp>

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram)
    : model(model), shaderProgram(shaderProgram)
{
}

DrawableObject::~DrawableObject() {
    delete model;
}

glm::mat4 DrawableObject::getModelMatrix() const {
    return transformation.getMatrix();
}

void DrawableObject::render() const {
    shaderProgram->setShaderProgram();
    shaderProgram->SetUniform("modelMatrix", getModelMatrix());
    model->draw();
}

Transformation& DrawableObject::getTransformation() {
    return transformation;
}

const Transformation& DrawableObject::getTransformation() const {
    return transformation;
}
