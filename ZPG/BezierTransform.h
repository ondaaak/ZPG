#pragma once
#include "Transformation.h"
#include <vector>

// Transformace pro kubickou Bézierovu SPLINE køivku.
class BezierTransform : public Transformation {
public:
    // Konstruktor pøijímá vektor kontrolních bodù.
    BezierTransform(const std::vector<glm::vec3>& points, float speed, bool loop = false);

    glm::mat4 getMatrix() const override;
    void update(float deltaTime);
    Transformation* clone() const override;

private:
    // Vypoèítá teèný vektor (smìr) pro daný parametr t.
    glm::vec3 evaluateTangent(float t, float& localT, int& segmentIndex) const;

    std::vector<glm::vec3> controlPoints; // Uložíme všechny body
    glm::mat4 bezierBaseMatrix;           // Bézierova bázová matice (A)

    float speed;
    float t;       // Parametr 't' nyní pobìží od 0 do poètu segmentù
    bool loop;
    float delta;   // Pro smìr pohybu
};