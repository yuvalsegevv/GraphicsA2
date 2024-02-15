#ifndef PLANE_H
#define PLANE_H

#include "glm/vec3.hpp"
#include "ray.h"
#include "HitObject.h"
#include <utility>


// Represents a plane in a 3D scene that can be intersected by rays
class plane : public HitObject {
public:
    // Default constructor
    plane() = default;

    // Constructor to initialize the plane with a normal vector, scalar, and material
    plane(glm::vec3 normal, float scalar)
        : normal(normal), scalar(scalar) {}

    // Checks if a ray intersects with the plane within a specified range
    virtual hit_rec get_hit(const ray& r, float t_min, float t_max, const material& objectMat) const = 0;
    virtual bool can_hit(const ray& incomingRay, float t_min, float t_max) const = 0;

private:
    glm::vec3 normal; // Normal vector of the plane
    float scalar;     // Distance from the origin to the plane along the normal vector
};

#endif
