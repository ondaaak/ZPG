#pragma once
#include <glm/glm.hpp>
#include "Subject.h"

class Light : public Subject {
public:
    Light(const glm::vec3& position, const glm::vec3& color)
        : position(position), diffuse(color),specular(glm::vec3(0.1f)), color(color) { }

    void setPosition(const glm::vec3& pos) {
        position = pos;
        notifyObservers(this);
    }
    void setDiffuse(const glm::vec3& diff) {
        diffuse = diff;
        notifyObservers(this);
    }
    void setSpecular(const glm::vec3& spec) {
        specular = spec;
        notifyObservers(this);
    }
    void setColor(const glm::vec3& col) {
        color = col;
        notifyObservers(this);
    }

    glm::vec3 getPosition() const { return position; }
	glm::vec3 getDiffuse() const { return diffuse; }
	glm::vec3 getSpecular() const { return specular; }
    glm::vec3 getColor() const { return color; }

private:
    glm::vec3 position;
    glm::vec3 diffuse;
    glm::vec3 specular;


    glm::vec3 color;

};
