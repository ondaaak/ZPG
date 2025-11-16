#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // Pøidáno pro jistotu

class Transformation {
public:
    virtual ~Transformation() {}
    virtual glm::mat4 getMatrix() const = 0;

    /**
     * Vytvoøí a vrátí ukazatel na novou (hlubokou) kopii této transformace.
     */
    virtual Transformation* clone() const = 0;
};