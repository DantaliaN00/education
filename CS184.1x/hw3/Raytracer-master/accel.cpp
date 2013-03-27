#include <vector>
#include "Ray.h"
#include "parse.h"
#include "geometry.h"
#include "accel.h"
#include "Box.h"

extern bool useAccelStructure;


SceneObject noAcceleration(Ray ray, float * t, SceneProperties scene) {
    //1. naive, acceleration-less option
    SceneObject closestObject;
    *t = maxT;

    for (vector<SceneObject>::iterator obj = scene.objects.begin(); obj != scene.objects.end(); obj++) {
        float currentT = obj->testIntersection(ray);
        if (currentT > 0.0 && currentT < *t) {
            *t = currentT;
            closestObject = *obj;
        }
    }

    return closestObject;
}


SceneObject acceleration(Ray ray, float * t, SceneProperties scene) {


    SceneObject * closest;
    float currentT = scene.accelStructure->testIntersection(ray,&closest);

    *t = currentT;
    return *closest;
}



//gets the closest object on the path of this ray
//and sets t to whatever the appropriate t value is
//will return a negative t if the object is behind us
SceneObject getClosestObject(Ray ray, float * t, SceneProperties scene) {

    if (useAccelStructure) {
        return acceleration(ray, t, scene);
    }
    
    else {
        return noAcceleration(ray, t, scene);
    }

}
