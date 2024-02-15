#ifndef HITTABLE_H
#define HITTABLE_H

#include "glm/vec3.hpp" // Ensure glm::vec3 is included
#include "ray.h"
// Material structure to represent material properties
struct material {
    glm::vec3 base_color;
    float reflective;
    float transparency;
    float Kd = 0.7f; // Default diffuse coefficient
    float shininess;

    // Default constructor for an uninitialized material
    material() = default;

    // Constructor to initialize a material with specific properties
    material(glm::vec3 color, float r, float t, float shiny)
        : base_color(color * 256.0f), reflective(r), transparency(t), shininess(shiny) {}
};

// Structure to record details of a ray-object intersection
struct hit_rec {
    glm::vec3 point; // Intersection point
    glm::vec3 normal; // Surface normal at the intersection
    float t; // Parameter along the ray where the intersection occurs
    material mat; // Material of the intersected object
};

// Abstract base class for hittable objects
class HitObject {
public:
    // Pure virtual functions to test and get intersection with a ray
    virtual hit_rec get_hit(const ray& r, float t_min, float t_max, const material& objectMat) const = 0;
    virtual bool can_hit(const ray& incomingRay, float t_min, float t_max) const = 0;
};

#endif
