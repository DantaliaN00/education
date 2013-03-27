#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <glm/glm.hpp>
#include "Ray.h"

using namespace std;
using namespace glm;

enum shape_t { SPHERE, TRI, TRINORMAL };

class VertexNormal {
 public:
  vec3 position;
  vec3 direction;
};

class SceneObject {

    public:

    SceneObject();
    ~SceneObject();

    int id;

    float alpha;

    bool operator!=(SceneObject &) const; 
    glm::mat4 transform;
    glm::mat4 invTrans;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    glm::vec3 emission;


    float testIntersection(Ray ray);
    glm::vec3 getNormal(glm::vec3 point);

    shape_t type;

    glm::vec3 center;
    float radius;

    //for triangle

    vec3 v1;
    vec3 v2;
    vec3 v3;

    //we reuse the Vertex type for a normal
    vec3 n1;
    vec3 n2;
    vec3 n3;
};

#endif
