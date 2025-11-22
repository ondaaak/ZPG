#include "PathTransform.h"
#include <glm/gtc/matrix_transform.hpp>

PathTransform::PathTransform(const std::vector<glm::vec3>& points, float speed)
    : points(points), speed(speed), t(0.0f), loop(false)
{
}

glm::mat4 PathTransform::getMatrix() const {
    glm::vec3 pos = evaluatePosition(t);
    return glm::translate(glm::mat4(1.0f), pos);
}

void PathTransform::update(float deltaTime) {
    if (points.size() < 2) return;
    t += speed * deltaTime;
    if (t > 1.0f) {
        if (loop) t = 0.0f;
        else t = 1.0f;
    }
}

glm::vec3 PathTransform::evaluatePosition(float t) const {
    if (points.empty()) return glm::vec3(0.0f);
    if (points.size() == 1) return points[0];

    float totalSegments = static_cast<float>(points.size() - 1);
    float segment = t * totalSegments;
    int idx = static_cast<int>(segment);
    float localT = segment - idx;

    if (idx >= points.size() - 1) return points.back();

    return glm::mix(points[idx], points[idx + 1], localT);
}