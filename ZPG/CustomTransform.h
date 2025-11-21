#pragma once
#include "Transformation.h"
#include <glm/glm.hpp>

class CustomTransform : public Transformation {
public:
    CustomTransform(float number = 1) : w(number) {}

    glm::mat4 getMatrix() const override {
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix[3][3] = w;
        return matrix;
    }

    virtual Transformation* clone() const override {
        return new CustomTransform(w);
    }

private:
    float w;
};

// Homogenni slozku pridavame pro zjednoduseni transformaci, diky ni muzeme pouzivat nasobeni matic pro vsechny transformace.
// V pripade pouziti cisla 1 jako homogenni slozky nemusime nic resit, v pripade jine hodnoty musime touto hodnotou vynasobit
// vsechny ostatni slozky a nasledne i vydelit touto hodnotou. V pripade pouziti nasi vlastni transformace s w=20 se objekt 
// zmensi 20x protoze zbyle slozky (x,y,z) jsou vydeleny 20.