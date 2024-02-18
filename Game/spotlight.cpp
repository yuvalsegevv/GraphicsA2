#include <iostream>
#include "spotlight.h"

spotlight::spotlight(glm::vec3 origin, glm::vec3 dir, float cut_off_angle, glm::vec3 i) {
    this->r = ray(origin, dir);
    this->cut_off_angle = cut_off_angle;
    this->baseIllumination = i;
}

float spotlight::getT(glm::vec3 point)  {
    glm::vec3 dir_to_point = point - r.orig;
    dir_to_point = glm::normalize(dir_to_point);
    return std::abs((r.orig.x - point.x) / dir_to_point.x);
}

glm::vec3 spotlight::getRay(glm::vec3 point)  {
    glm::vec3 dir_to_point = point - r.orig;
    dir_to_point = glm::normalize(dir_to_point);
    float degree = std::acos(glm::clamp(glm::dot(dir_to_point, r.dir), -0.999f, .9999f));
    if (degree > cut_off_angle * 3.1415 * 2)
        return glm::vec3(0.0f, 0.0f, 0.0f);
    else return dir_to_point * -1.f;
}

glm::vec3 spotlight::getIllu(ray camera, hit_rec obj)  {
    glm::vec3 x = getRay(obj.point);
    if (glm::length(x) == 0)
        return glm::vec3(.0f, .0f, .0f);
    glm::vec3 L = glm::normalize(x);
    glm::vec3 V = glm::normalize(camera.dir * (-1.0f));
    glm::vec3 N = glm::normalize(obj.normal);
    glm::vec3 R = glm::normalize(2.0f * glm::dot(N, L) * N - L);
    return (obj.mat.Kd * glm::dot(L, N) + specularReflection * std::fmax(0.0f, ((float)std::pow((float)glm::dot(R, V), obj.mat.shininess)))) * baseIllumination;
}