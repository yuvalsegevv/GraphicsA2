#ifndef HITTABLE_H
#define HITTABLE_H

#include "glm/vec3.hpp" // Ensure glm::vec3 is included
#include "glm/vec4.hpp" // Ensure glm::vec4 is included
#include "ray.h"
// Material structure to represent material properties
struct material {
    glm::vec3 base_color;
    bool reflective;
    bool transparent;
    float Kd = 0.7f; // Default diffuse coefficient
    float shininess;
    // Normal vector of the plane
    // Distance from the origin to the plane along the normal vector
    // Default constructor for an uninitialized material
    material() = default;

    // Constructor to initialize a material with specific properties
    material(glm::vec3 color, bool r, bool t, float shiny)
        : base_color(color * 255.0f), reflective(r), transparent(t), shininess(shiny) {}
};

// Structure to record details of a ray-object intersection
struct hit_rec {
    bool hit;
    glm::vec3 point; // Intersection point
    glm::vec3 normal; // Surface normal at the intersection
    float t; // Parameter along the ray where the intersection occurs
    material mat; // Material of the intersected object
};

// Abstract base class for hittable objects
class HitObject {
public:
    glm::vec4 coords;
    material mat;
    HitObject(glm::vec3 base_color, glm::vec4 coords, float shiny, bool reflective, bool transparent) : mat(base_color, reflective, transparent, shiny), coords(coords) {};
    // Pure virtual functions to test and get intersection with a ray
    hit_rec get_hit(const ray& r, float t_min, float t_max);};

#endif
