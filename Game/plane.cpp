/*#include <algorithm>
#include "plane.h"
#include "glm/detail/func_geometric.hpp"

bool plane::can_hit(const ray& incomingRay, float t_min, float t_max) const {
    glm::vec3 normalized_normal = glm::normalize(normal);
    float denom = glm::dot(normalized_normal, incomingRay.dir);

    if (std::abs(denom) > 1e-6) { // Check if ray is not parallel to the plane
        float t = (glm::dot(-incomingRay.orig, normalized_normal) + scalar) / denom;
        return t >= t_min && t <= t_max; // Return true if t is within the range
    }

    return false; // Return false if ray is parallel to the plane or t is out of range
}

hit_rec plane::get_hit(const ray& incomingRay, float t_min, float t_max, const material& objectMat) const {
    hit_rec rec;
    glm::vec3 normal_menormal = glm::normalize(normal);
    float denom = glm::dot(normal_menormal, incomingRay.dir);

    // we directly calculate t
    float t = (glm::dot(-incomingRay.orig, normal_menormal) + scalar) / denom;

    // Populate the hit_record
    rec.t = t;
    rec.point = incomingRay.at(t);
    rec.normal = normal_menormal;
    rec.mat = objectMat;

    // Apply checker pattern modification
    glm::vec3 projected_point = glm::vec3(rec.point.x, rec.point.y, 0); // Assuming z=0 for simplicity
    bool is_even = (static_cast<int>(std::floor(projected_point.x * 1.5) + std::floor(projected_point.y * 1.5)) % 2) == 0;
    rec.mat.Kd = is_even ? 0.5f : objectMat.Kd;

    return rec; // Return the populated hit_record
}
*/