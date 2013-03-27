#include <math.h>
#include <sstream> 
#include <iostream>
#include <cmath>
#include <assert.h>
#include "util.h"
#include "Ray.h"
#include "raytrace.h"
#include "parse.h"
#include "accel.h"

using namespace glm;
extern SceneProperties scene; 
extern bool debugSingleRay;
extern int singleRayX; extern int singleRayY;
extern bool useAccelStructure;

extern bool noSpecular;
extern bool noDiffuse;
extern bool noEmission;
extern bool noAmbient;
extern bool reflectRays;

int bounceCount;

//we establish a maximum t value 
//that is hopefully high enough that
//we'll never have to worry about it
const float selfShadowTolerence = 0.001;

vec3 shootObjectRay(Ray);
vec3 shootLightingRays(vec3,Ray, SceneObject);

float max(float a, float b) {
    if (a >= b) { return a; }
    else { return b; }
}

vec3 correctToOne(vec3 input) {

    vec3 output(0,0,0);

    //clamps the output to between 1.0 and 0.0

    output.r = (input.r <= 1.0) ? input.r : 1.0;
    output.g = (input.g <= 1.0) ? input.g : 1.0;
    output.b = (input.b <= 1.0) ? input.b : 1.0;

    output.r = (input.r < 0.0) ? 0.0 : output.r;
    output.g = (input.g < 0.0) ? 0.0 : output.g;
    output.b = (input.b < 0.0) ? 0.0 : output.b;
    return output;
}

std::ostream& operator<<(std::ostream& output, const Ray& r) {
    output << "Orig: " << r.origin.x << "," << r.origin.y << "," << r.origin.z << "\n";
    output << "Dir: " << r.direction.x << "," << r.direction.y << "," << r.direction.z << "\n";
    return output;

}

//calculates the initial ray passing through a screen pixel
//into the screne
Ray generateRay(float i, float j) {
    Ray r;

    //set origin
    r.origin.x = scene.cameraParams.lookFromX;
    r.origin.y = scene.cameraParams.lookFromY;
    r.origin.z = scene.cameraParams.lookFromZ;

    //this math is from slide 16

    //'A' is eye - center, but center is 0 so whatever?
    vec3 W = normalize(vec3( 
                            scene.cameraParams.lookFromX 
                            - scene.cameraParams.lookAtX,
                            scene.cameraParams.lookFromY 
                            - scene.cameraParams.lookAtY,
                            scene.cameraParams.lookFromZ 
                            - scene.cameraParams.lookAtZ ));
			  
    //B is the up vector
    vec3 B = vec3(scene.cameraParams.upX, scene.cameraParams.upY, scene.cameraParams.upZ);
    vec3 U = normalize(cross(B,W));

    vec3 V = cross(W,U);

    float fovy = scene.cameraParams.fov;

    float w = (float) scene.width; float h = (float) scene.height;
    float fovx = atan( (w / h) * tan(fovy * 3.14159 / 180.0)) * 180.0 / 3.14159;

    //float fovx = fovy * ((float) scene.width / (float) scene.height);

    float dInvX = tan((fovx / 2.0) * (3.141592653589793/180.0));
    float dInvY = tan((fovy / 2.0) * (3.141592653589793/180.0));
    float alpha = dInvX * ((i - (scene.width / 2.0)) / (scene.width / 2.0));
    float beta = dInvY * (((scene.height / 2.0) - j) / (scene.height / 2.0));

    U = alpha*U;
    V = beta*V;

    r.direction = normalize(U + V - W);
    return r;
}

Ray calculateReflectedRay(Ray incoming, vec3 normal, vec3 surfacePoint) {

    Ray outgoing = Ray();
    outgoing.origin = surfacePoint;

    //cf. http://paulbourke.net/geometry/reflected/
    vec3 ri = incoming.direction;
    outgoing.direction = ri - 2.0f * normal * dot(normal, ri);
    return outgoing;
}


SceneObject getObjectAtPixel(int x, int y, float *outert) {

    Ray rayFromPixel = generateRay(x,y);
    float t;
    SceneObject obj = getClosestObject(rayFromPixel, &t, scene);
    *outert = t;
    return obj;

}


vec3 traceRay(float screenx, float screeny) {
    //cf http://www.codermind.com/articles/Raytracer-in-C++-Part-I-First-rays.html


    if (debugSingleRay) 
        cout << "Debugging single ray at coordinates: " << singleRayX << ", " << singleRayY << "\n";

    //generate the ray
    Ray thisRay = generateRay(screenx, screeny);
    if (debugSingleRay) {
        cout << "Our ray is: " << thisRay << "\n";
    }

    //set bounceCount to whatever the maxdepth is
    bounceCount = scene.maxdepth;

    //shootRay is the workhorse of the ray tracer
    vec3 finalColor = shootObjectRay(thisRay);

    //disable debugging
    debugSingleRay = false;
    return correctToOne(finalColor);
}


//shootObjectRay iterates over objects in scene and detects the closest
//one from the origin of this ray
vec3 shootObjectRay(Ray ray) {

    vec3 outputColor = scene.backgroundColor; //init to backgroundColor

    float t;
    SceneObject closestObject = getClosestObject(ray, &t, scene);


    if (t > 0.0 && t < maxT) { //we only compute lighting if there is in fact an intersection
        vec3 newOrigin = ray.getTLocation(t);

        outputColor = shootLightingRays(newOrigin,ray, closestObject); 

        if (reflectRays && bounceCount > 0) {
            //cout << "reflecting! " << bounceCount << "\n";
            bounceCount--;

            Ray reflectRay = calculateReflectedRay(ray,closestObject.getNormal(newOrigin), newOrigin);
            outputColor += closestObject.specular *shootObjectRay(reflectRay);
        }

        if (noEmission == false)
            outputColor += closestObject.emission;

        if (noAmbient == false)
            outputColor += closestObject.ambient;

        if (closestObject.alpha < 1.00) {
            float alp = closestObject.alpha;
            Ray newRay;
            newRay.origin = newOrigin + 0.001f*ray.direction;
            newRay.direction = ray.direction;
            vec3 colorNext = shootObjectRay(newRay);
            //colorNext = vec3(0.0f,0.0f,0.0f);
            outputColor  = alp*outputColor + (1.0f-alp)*colorNext;
        }
    }

    return outputColor; //if the ray never intersected an object this gets set to background
}



//shootLightingRays iterates over all the lights in the scene
//that aren't blocked by something, and returns a color
vec3 shootLightingRays(vec3 surfacePoint, Ray origRay, SceneObject object) {

    //n.b. attenuation is only for point lights

    vec3 diffuseComponent(0,0,0);
    vec3 specularComponent(0,0,0);

    vec3 normal = object.getNormal(surfacePoint);
    if (debugSingleRay) 
        debugVector(normal, "Here is the normal: ");


    //first calculate a vector of lights visible from this point

    for (vector<Light>::iterator light = scene.lights.begin(); light != scene.lights.end(); light++) {

        //determine whether the light is directional
        vec3 lightVector;
        if (light->directional)
            lightVector = vec3(-1,-1,-1)*light->location;
        else
            lightVector = light->location - surfacePoint;

        //we generate a ray from the surface point towards the light
        Ray ray;
        ray.origin = surfacePoint;
        ray.direction = normalize(lightVector);

        //first shoot a ray from the point to the light, and if something's in the way, do nothing

        float lightT = light->testIntersection(ray); 
        //lightT should never be negative 
        float t = lightT;
        assert(t >= 0.0);
        SceneObject closestObject;
        for (vector<SceneObject>::iterator obj = scene.objects.begin(); obj != scene.objects.end(); obj++) {
            float currentT = obj->testIntersection(ray);
            if (currentT > 0.0 && currentT < t) {
                t = currentT;
                closestObject = *obj;
            }
        }

        if (t < lightT) { //if t is less than maxT there's an object in the way
          if (t > selfShadowTolerence) { //but we don't want to shadow ourselves
              continue; //with this continue we skip over this light 
          }
        }

        //attenuation
        vec3 attenuatedColor = light->color;
        if (light->directional != true) {
            //TODO have an "attenuation distance scaling" global
            lightT *= 0.5; //trying to scale the distance so we don't get crazy dim lights
            float attenFactor = 1.0 / (light->constAttenuation + light->linearAttentuation * lightT + light->quadAttenuation * (lightT * lightT));

            //we don't want attenuation to ever increase the amount of light
            attenFactor = max(attenFactor, 1.0);

            attenuatedColor = attenuatedColor * attenFactor;
        }

        //diffuse component
        vec3 L = normalize(lightVector);
        diffuseComponent += object.diffuse * max(dot(L,normal),(float) 0.0)*attenuatedColor;

        //specular component 
        vec3 vectorToEye = normalize(origRay.origin - surfacePoint);
        vec3 vectorToLightSource = normalize(lightVector);
        vec3 halfAngle = normalize(vectorToLightSource + vectorToEye);

        specularComponent += object.specular * pow( max( dot(normal, halfAngle), 0.0f ), object.shininess) * attenuatedColor;
    }

    
    vec3 returnVector(0.0f,0.0f,0.0f);
    if (noSpecular == false)
        returnVector += specularComponent;
    if (noDiffuse == false)
        returnVector += diffuseComponent;
    return returnVector;
}


