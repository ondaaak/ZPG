#include "DrawableObject.h"

DrawableObject::DrawableObject(const float* vertices, size_t vertexCount, ShaderProgram* shaderProgram)
    : shaderProgram(shaderProgram),
    VBO(0),
    VAO(0),
    count(static_cast<GLsizei>(vertexCount / 3)) 
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(0);
}

DrawableObject::~DrawableObject() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void DrawableObject::render() const {
    if (shaderProgram) {
        shaderProgram->setShaderProgram();
    }


    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, count);
    glBindVertexArray(0);
}
