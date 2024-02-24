#include <iostream>
#include "spotlight.h"

spotlight::spotlight(glm::vec3 origin, glm::vec3 dir, float cut_off_angle, glm::vec3 i) {
    this->r = ray(origin, dir);
    this->cut_off_angle = cut_off_angle;
    this->illumination = i;
}

float spotlight::getT(glm::vec3 point)  {
    glm::vec3 dir_to_point = point - r.orig;
    dir_to_point = glm::normalize(dir_to_point);
    return std::abs((r.orig.x - point.x) / dir_to_point.x);
}

glm::vec3 spotlight::getRay(glm::vec3 point)  {
    glm::vec3 dir_to_point = point - r.dir;
    dir_to_point = glm::normalize(dir_to_point);
    
    float dotProduct = glm::dot(point - r.dir, r.orig);

    // Calculate the magnitudes (lengths) of the vectors
    float magV1 = glm::length(point - r.dir);
    float magV2 = glm::length(r.orig);

    // Ensure denominators are not zero
    if (magV1 == 0.0f || magV2 == 0.0f) {
        std::cerr << "Error: Magnitude of a vector cannot be zero." << std::endl;
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    // Calculate the cosine of the angle
    float cosTheta = dotProduct / (magV1 * magV2);

    // Clamp the cosine value to the valid range [-1, 1] to avoid floating-point precision issues
    cosTheta = glm::clamp(cosTheta, -1.0f, 1.0f);


    if (cosTheta < cut_off_angle)
        return glm::vec3(0.0f, 0.0f, 0.0f);
    else return r.dir - point;
}