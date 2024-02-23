#include <iostream>
#include "dirlight.h"

dirlight::dirlight(glm::vec3 dir, glm::vec3 i) {
    this->dir = dir;
    this->baseIllumination = i;
}

float dirlight::getT(glm::vec3 point)  {
    return 99999999.9;
}

glm::vec3 dirlight::getRay(glm::vec3 point)  {
    return dir * -1.0f;
}

glm::vec3 dirlight::getIllu(ray reflected, hit_rec obj, ray camera)  {
    if (glm::length(reflected.dir) == 0)
        return glm::vec3(.0f, .0f, .0f);
    glm::vec3 L = glm::normalize(reflected.dir);
    glm::vec3 N = glm::normalize(obj.normal);
    glm::vec3 R = glm::normalize(2.0f * glm::dot(N, L) * N - L);
    glm::vec3 V = glm::normalize(camera.dir);
    glm::vec3 diffuse = obj.mat.Kd * std::abs(glm::dot(N, L)) * baseIllumination;
    glm::vec3 specular = obj.mat.Kd * std::pow(glm::dot(R, V), obj.mat.shininess) * baseIllumination;
    return diffuse + specular;
}