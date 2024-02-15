#include <iostream>
#include "light.h"

glm::vec3 light::getIllu(ray camera, hit_rec obj) const {
    glm::vec3 x = getRay(obj.point);
    if (glm::length(x) == 0)
        return glm::vec3(.0f, .0f, .0f);
    glm::vec3 L = glm::normalize(x);
    glm::vec3 V = glm::normalize(camera.dir * (-1.0f));
    glm::vec3 N = glm::normalize(obj.normal);
    glm::vec3 R = glm::normalize(2.0f * glm::dot(N, L) * N - L);
    // TODO no such thing as diffuseReflection in the material structure
    return R; // TODO change it!!!!!
   //return (obj.mat.diffuseReflection * glm::dot(L, N) + specularReflection * std::fmax(0.0f, ((float)std::pow((float)glm::dot(R, V), obj.mat.shininess)))) * baseIllumination;
}