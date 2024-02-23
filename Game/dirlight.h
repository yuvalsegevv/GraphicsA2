#include "ray.h"
#include "eye.h"
#include "HitObject.h"
#include "glm/detail/func_geometric.hpp"
static float diffuseReflection_ = 0.7f;
static float ambientReflection_ = 0.7f;
static float specularReflection_ = 0.7f;

class dirlight {
public:
    
    glm::vec3 baseIllumination;
    glm::vec3 dir;
    dirlight() {}
    dirlight(glm::vec3 dir, glm::vec3 i);
    glm::vec3 getRay(glm::vec3 point);
    float getT(glm::vec3 point);
    glm::vec3 getIllu(ray reflected, hit_rec obj, ray camera) ;
};
