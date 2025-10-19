#pragma once
#include <glm/glm.hpp>

class Light {
public:
    Light(const glm::vec3& position, const glm::vec3& color)
        : position(position), color(color) {}

    void setPosition(const glm::vec3& pos) { position = pos; }
    glm::vec3 getPosition() const { return position; }

    void setColor(const glm::vec3& col) { color = col; }
    glm::vec3 getColor() const { return color; }

private:
    glm::vec3 position;
    glm::vec3 color;
};
