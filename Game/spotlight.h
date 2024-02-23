#include "ray.h"
#include "eye.h"
#include "HitObject.h"
#include "glm/detail/func_geometric.hpp"
static float diffuseReflection = 0.7f;
static float ambientReflection = 0.7f;
static float specularReflection = 0.7f;

class spotlight {
public:
    
    glm::vec3 baseIllumination;
    ray r;
    float cut_off_angle;
    spotlight() {}
    spotlight(glm::vec3 origin, glm::vec3 dir, float cut_off_angle, glm::vec3 i);
    glm::vec3 getRay(glm::vec3 point);
    float getT(glm::vec3 point);
    glm::vec3 getIllu(ray reflected, hit_rec obj, ray camera) ;
};
