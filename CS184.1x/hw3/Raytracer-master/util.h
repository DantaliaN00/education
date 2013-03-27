#ifndef UTIL_H
#define UTIL_H

#include <glm/glm.hpp>
#include <string>
#include <iostream>

inline void debugVector(glm::vec3 vector, std::string msg) {
    std::cout << msg << ": " << vector.x << "," << vector.y << "," << vector.z << "\n";
}

#endif
