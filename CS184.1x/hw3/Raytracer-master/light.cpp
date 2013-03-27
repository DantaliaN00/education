
#include "light.h"
#include <cmath>

using namespace glm;

float Light::testIntersection(Ray ray) {

  if(this->directional){
    return 9999999;
  } else {
    vec3 connection = this->location - ray.origin;
    return sqrt(dot(connection, connection));
  }
}

//sane defaults
Light::Light() {
    //default is no attenuation
    this->constAttenuation = 1;
    this->linearAttentuation = 0;
    this->quadAttenuation = 0;
}

Light::~Light() {

}
