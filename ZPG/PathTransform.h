#pragma once
#include "Transformation.h"
#include <glm/glm.hpp>
#include <vector>

class PathTransform : public Transformation {
public:
    PathTransform(const std::vector<glm::vec3>& points, float speed = 1.0f);

    glm::mat4 getMatrix() const override;
    virtual Transformation* clone() const override { return new PathTransform(points, speed); }

    void update(float deltaTime);
    void setLoop(bool loop) { this->loop = loop; }
    void reset() { t = 0.0f; }

   

private:
    std::vector<glm::vec3> points;  
    float speed;                     
    float t;                         
    bool loop;  

    glm::vec3 evaluatePosition(float t) const;
};