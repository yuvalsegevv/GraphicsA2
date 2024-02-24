#include <algorithm>
#include "HitObject.h"
#include "glm/detail/func_geometric.hpp"

hit_rec HitObject::get_hit(const ray& incomingRay, float t_min, float t_max) {
    if (coords.w <= 0) {

        float t = -(coords.x * incomingRay.orig.x + coords.y * incomingRay.orig.y + coords.z * incomingRay.orig.z + coords.w) /
            (coords.x * incomingRay.dir.x + coords.y * incomingRay.dir.y + coords.z * incomingRay.dir.z);

        glm::vec3 intersection = incomingRay.orig + t * incomingRay.dir;

        
        hit_rec rec;
        glm::vec3 normal_menormal = glm::vec3(coords.x, coords.y, coords.z);
        if (isnan(intersection.x) || t < 0)
            rec.t = 99999999.9f;
        else
            rec.t = glm::length(incomingRay.orig - intersection);
        rec.point = intersection;
        rec.normal = normal_menormal;
        rec.mat = mat;

        // Apply checker pattern modification
        glm::vec3 projected_point = glm::vec3(rec.point.x, rec.point.y, 0); // Assuming z=0 for simplicity
        bool is_even = (static_cast<int>(std::floor(projected_point.x * 2) + std::floor(projected_point.y * 2)) % 2) == 0;
        rec.mat.Kd = is_even ? mat.Kd : mat.Kd * 0.5f;
        return rec; // Return the populated hit_record*/
    }
    else {
        hit_rec rec;
        glm::vec3 sphere_center = incomingRay.orig - glm::vec3(coords.x, coords.y, coords.z);
        float a = incomingRay.dir[0] * incomingRay.dir[0] + incomingRay.dir[1] * incomingRay.dir[1] + incomingRay.dir[2] * incomingRay.dir[2];
        auto half_b = glm::dot(sphere_center, incomingRay.dir);
        float c = glm::dot(sphere_center, sphere_center) - coords.w * coords.w;
        float discriminant = half_b * half_b - a * c;

        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range
        auto root = (-half_b - sqrtd) / a;
        if (root < t_min || t_max < root) {
            root = (-half_b + sqrtd) / a;
        }
        if (isnan(root))
            rec.t = 99999999.9f;
        else
            rec.t = root;
        rec.point = incomingRay.at(rec.t);
        rec.normal = (rec.point - glm::vec3(coords.x, coords.y, coords.z)) / coords.w;
        rec.mat = mat;

        return rec;
    }
}
