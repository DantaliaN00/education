
#ifndef RAY_H
#define RAY_H
#include <glm/glm.hpp>
//note that rays have normalized directions
class Ray {
    public:
    glm::vec3 origin;

    glm::vec3 direction;

    glm::vec3 getTLocation(float t) {
        return origin + direction*t;
    }
};

#endif
