#include "BezierTransform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath> 

BezierTransform::BezierTransform(const std::vector<glm::vec3>& points, float speed, bool loop)
    : controlPoints(points), speed(speed), t(0.0f), loop(loop), delta(1.0f)
{
    bezierBaseMatrix = glm::mat4(
        glm::vec4(-1.0, 3.0, -3.0, 1.0),
        glm::vec4(3.0, -6.0, 3.0, 0.0),
        glm::vec4(-3.0, 3.0, 0.0, 0.0),
        glm::vec4(1.0, 0.0, 0.0, 0.0)
    );
}

void BezierTransform::update(float deltaTime) {
    if (controlPoints.size() < 4) return;
    int numSegments = (controlPoints.size() - 1) / 3;
    if (numSegments == 0) return;

    t += speed * deltaTime * delta;

    if (t > numSegments) {
        if (loop) { t = fmod(t, (float)numSegments); }
        else { t = (float)numSegments; delta *= -1.0f; }
    }
    else if (t < 0.0f) {
        if (loop) { t += (float)numSegments; }
        else { t = 0.0f; delta *= -1.0f; }
    }
}

glm::vec3 BezierTransform::evaluateTangent(float t, float& localT, int& segmentIndex) const {
    segmentIndex = static_cast<int>(floor(t));
    int numSegments = (controlPoints.size() - 1) / 3;
    if (segmentIndex >= numSegments) {
        segmentIndex = numSegments - 1;
    }
    localT = t - segmentIndex;

    int p0_idx = segmentIndex * 3;
    glm::vec3 p0 = controlPoints[p0_idx];
    glm::vec3 p1 = controlPoints[p0_idx + 1];
    glm::vec3 p2 = controlPoints[p0_idx + 2];
    glm::vec3 p3 = controlPoints[p0_idx + 3];

    // Derivace kubické Bézierovy køivky
    float oneMinusT = 1.0f - localT;
    return 3.0f * oneMinusT * oneMinusT * (p1 - p0) +
        6.0f * oneMinusT * localT * (p2 - p1) +
        3.0f * localT * localT * (p3 - p2);
}

glm::mat4 BezierTransform::getMatrix() const {
    if (controlPoints.size() < 4) return glm::mat4(1.0f);

    float localT;
    int segmentIndex;

    glm::vec3 tangent = evaluateTangent(t, localT, segmentIndex);
    if (glm::length(tangent) == 0.0f) {
        return glm::mat4(1.0f);
    }

    int p0_idx = segmentIndex * 3;
    glm::vec3 p0 = controlPoints[p0_idx];
    glm::vec3 p1 = controlPoints[p0_idx + 1];
    glm::vec3 p2 = controlPoints[p0_idx + 2];
    glm::vec3 p3 = controlPoints[p0_idx + 3];

    glm::mat4x3 currentSegmentMatrix = glm::mat4x3(p0, p1, p2, p3);
    glm::vec4 parameters = glm::vec4(localT * localT * localT, localT * localT, localT, 1.0f);
    glm::vec3 position = parameters * bezierBaseMatrix * glm::transpose(currentSegmentMatrix);

    glm::mat4 rotationMatrix = glm::inverse(glm::lookAt(
        position,
        position + tangent,
        glm::vec3(0.0f, 1.0f, 0.0f)
    ));

    return rotationMatrix;
}

Transformation* BezierTransform::clone() const {
    return new BezierTransform(controlPoints, speed, loop);
}