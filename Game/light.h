#ifndef GAME_LIGHT_H
#define GAME_LIGHT_H

#include "ray.h"
#include "eye.h"
#include "HitObject.h"
#include "glm/detail/func_geometric.hpp"
static float diffuseReflection = 0.7f;
static float ambientReflection = 0.7f;
static float specularReflection = 0.7f; 

class light {
public:
    light() = default;;
    virtual float getT(glm::vec3 point) const = 0;
    virtual glm::vec3 getRay(glm::vec3 point) const = 0;
    virtual glm::vec3 getIllu(ray camera, hit_rec obj) const;
    glm::vec3 baseIllumination;
};

#endif 