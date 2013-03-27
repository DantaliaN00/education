#include "geometry.h"
#include "util.h"
#include "math.h"
#include <iostream>

using namespace glm;


bool SceneObject::operator!=(SceneObject & object) const { 

    return (object.id != this->id);
}


//set up some sane defaults
SceneObject::SceneObject() {
    this->id = 0;
    this->ambient = vec3(1.0,0.2,0.2);
    this->emission = vec3(0.0,0.0,0.0);
    this->diffuse = vec3(1.0, 1.0, 1.0);
    this->specular = vec3(0,0,0);
    this->shininess = 50;
    this->alpha = 1.0;
//    this->diffuse = vec3(0.5 0.5 0.5);

}

SceneObject::~SceneObject() {

}



vec3 SceneObject::getNormal(glm::vec3 point) {

    if (this->type == SPHERE) {
      return normalize(point-vec3(this->transform*vec4(this->center,1.0f)));
    } else if( this->type == TRI ) {
      return normalize(cross(v2-v1,v3-v1));
    } else if( this->type == TRINORMAL ){
      float 
        alphaA = sqrt(dot(point - n1, point - n1)),
        betaA = sqrt(dot(point - n2, point - n2)),
        gammaA = sqrt(dot(point - n3, point - n3));
      float
        alpha = alphaA / (alphaA + betaA + gammaA),
        beta  = betaA / (alphaA + betaA + gammaA),
        gamma = gammaA / (alphaA + betaA + gammaA);
      
      return alpha * n1 + beta * n2 + gamma * n3;
    } else {
      std::cout << "ERROR: invalid shape type\n";
    }
}

//this is an abuse of polymorphism...
//anyway sceneObject needs to test itself to know if it 
//is a sphere or not
float SceneObject::testIntersection(Ray ray) {

  if (this->type == SPHERE) {
    
    //math pulled from textbook p. 77
    vec3 e = vec3(this->invTrans*vec4(ray.origin,1.0f));
    vec3 d = vec3(this->invTrans*vec4(ray.direction, 0.0f));
    vec3 c = this->center;
   
        //debugVector(d, "Debug d: ");
    
        //note that the var discriminant is actually the discriminant before
        //we sqrt it - just keep that in mind in the future
    double discriminant = dot(d,(e-c))*dot(d,(e-c)) - dot(d,d)*(dot((e-c),(e-c)) - (this->radius*this->radius));

        float tol = 0.0001;
        if (0 + tol > discriminant && 0 - tol < discriminant) {
            //discriminant 0, ray grazes sphere
            float returnVal = -1 * dot(d,(e-c)) / dot(d,d);
            returnVal = -1;
            return returnVal;
        }

        else if (discriminant > 0) {

            float t1 = (-1 * dot(d,(e-c)) + sqrt(discriminant)) / dot(d,d);
            float t2 = (-1 * dot(d,(e-c)) - sqrt(discriminant)) / dot(d,d);
            
            if (t1 > t2) { return t2; } else { return t1; }
        }

        else if (discriminant < 0) { 
            //complex discriminant, ray doesn't touch object
            //so return -1 so it won't get drawn
            return -1.0;
        }

        return 1.0;
    }
    else { //if it's not a sphere it's a triangle or trinormal
      vec3 
        a = vec3(this->v1.x, this->v1.y, this->v1.z),
        b = vec3(this->v2.x, this->v2.y, this->v2.z),
        c = vec3(this->v3.x, this->v3.y, this->v3.z);
        
      vec3 normal = normalize(cross(b-a, c-a));
      if( dot(normal, ray.direction) == 0.0 )
        return -1.0;
      if( dot(normal, ray.direction) > 0.0 ){
        vec3 tmp = a;
        a = b;
        b = tmp;
        normal = -normal;
      }

      vec3 rayStart = vec3(ray.origin.x,ray.origin.y, ray.origin.z);

      float 
        returnVal = dot(normal, a - rayStart) / dot(normal, ray.direction);
      
      float tol = 0.0001;
      if(returnVal < tol){
        return -1.0;
      }
      vec3 x = rayStart + returnVal * ray.direction;
      
      if( dot(cross(b - a, x - a), normal) >= 0 &&
          dot(cross(c - b, x - b), normal) >= 0 &&
          dot(cross(a - c, x - c), normal) >= 0 )
        return returnVal;

      return -1.0;

    }
}

