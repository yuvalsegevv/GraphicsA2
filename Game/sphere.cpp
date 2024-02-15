#include "sphere.h"
#include "glm/detail/func_geometric.hpp"
#include "ray.h"




bool sphere::can_hit(const ray& incomingRay, float t_min, float t_max) const {
    glm::vec3 sphere_center = incomingRay.orig - center;
    float a = incomingRay.dir[0] * incomingRay.dir[0] + incomingRay.dir[1] * incomingRay.dir[1] + incomingRay.dir[2] * incomingRay.dir[2];
    auto half_b = glm::dot(sphere_center, incomingRay.dir);
    float c = glm::dot(sphere_center, sphere_center) - radius * radius;
    float discriminant = half_b * half_b - a * c;

    if (discriminant < 0) return false;

    // Check for the nearest root in the acceptable range
    auto sqrtd = sqrt(discriminant);
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    return true;
}


hit_rec sphere::get_hit(const ray& incomingRay, float t_min, float t_max, const material& objectMat) const {
    hit_rec rec;
    glm::vec3 sphere_center = incomingRay.orig - center;
    float a = incomingRay.dir[0] * incomingRay.dir[0] + incomingRay.dir[1] * incomingRay.dir[1] + incomingRay.dir[2] * incomingRay.dir[2];
    auto half_b = glm::dot(sphere_center, incomingRay.dir);
    float c = glm::dot(sphere_center, sphere_center) - radius * radius;
    float discriminant = half_b * half_b - a * c;

    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range
    auto root = (-half_b - sqrtd) / a;
    rec.t = root;
    rec.point = incomingRay.at(rec.t);
    rec.normal = (rec.point - center) / radius;
    rec.mat = objectMat;

    return rec;
}
