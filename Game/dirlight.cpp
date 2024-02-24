#include <iostream>
#include "dirlight.h"

dirlight::dirlight(glm::vec3 dir, glm::vec3 i) {
    this->dir = dir;
    this->illumination = i;
}

float dirlight::getT(glm::vec3 point)  {
    return 99999999.9;
}

glm::vec3 dirlight::getRay(glm::vec3 point)  {
    return dir * -1.0f;
}