#ifndef SPHERE_H
#define SPHERE_H

#include "HitObject.h" // Updated to include the new base class
#include "glm/vec3.hpp" // Ensure glm::vec3 is included
#include "ray.h"
#include <optional>

class sphere : public HitObject {
public:
    glm::vec3 center;
    float radius;

    // Default constructor
    sphere() : center(glm::vec3(0.0f)), radius(1.0f) {}

    // Constructor with initialization
    sphere(glm::vec3 cen, float r) : center(cen), radius(r) {}

    // Implementation of HitObject's pure virtual functions
    bool can_hit(const ray& incomingRay, float t_min, float t_max) const override;
    hit_rec get_hit(const ray& r, float t_min, float t_max, const material& objectMat) const override;
};

#endif
