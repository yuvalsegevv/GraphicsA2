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

glm::vec3 dirlight::getIllu(ray camera, hit_rec obj)  {
    glm::vec3 x = getRay(obj.point);
    if (glm::length(x) == 0)
        return glm::vec3(.0f, .0f, .0f);
    glm::vec3 L = glm::normalize(x);
    glm::vec3 V = glm::normalize(camera.dir * (-1.0f));
    glm::vec3 N = glm::normalize(obj.normal);
    glm::vec3 R = glm::normalize(2.0f * glm::dot(N, L) * N - L);
    return (obj.mat.Kd * glm::dot(L, N) + specularReflection_ * std::fmax(0.0f, ((float)std::pow((float)glm::dot(R, V), obj.mat.shininess)))) * baseIllumination;
}