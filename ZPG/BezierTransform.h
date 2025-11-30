#pragma once
#include "Transformation.h"
#include <vector>

class BezierTransform : public Transformation {
public:
    BezierTransform(const std::vector<glm::vec3>& points, float speed, bool loop = false);

    glm::mat4 getMatrix() const override;
    void update(float deltaTime);
    Transformation* clone() const override;
	void setControlPoints(const std::vector<glm::vec3>& points);

private:
    glm::vec3 evaluateTangent(float t, float& localT, int& segmentIndex) const;

    std::vector<glm::vec3> controlPoints; 
    glm::mat4 bezierBaseMatrix; 

    float speed;
    float t;    
    bool loop;
    float delta; 
};