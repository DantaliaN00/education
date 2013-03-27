#include <glm/glm.hpp>
#include "Ray.h"

class Light {
    public:

        Light();
        ~Light();

        bool directional;

        glm::vec3 location;
        glm::vec3 color;

        float constAttenuation;
        float linearAttentuation;
        float quadAttenuation;

        float testIntersection(Ray);
};

