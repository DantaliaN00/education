#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include "geometry.h"
#include "light.h"
#include "Box.h"

using namespace std;

class SceneProperties {
    public:

        SceneProperties();
        ~SceneProperties();

        Box * accelStructure;

        int height;
        int width;
        int maxdepth;

        int maxvertnorms;
        int maxverts;

        vector<vec3> vertices;
        vector<VertexNormal> normalvertices;


        string output;

        struct {
            float lookFromX; float lookFromY; float lookFromZ;
            float lookAtX; float lookAtY; float lookAtZ; 
            float upX; float upY; float upZ;
            float fov;
        } cameraParams;

        glm::vec3 backgroundColor;

        void insertObject(SceneObject object);

        vector<glm::mat4> transformStack;
        vector<glm::mat4> matrixStack;
        vector<SceneObject> objects;
        vector<Light> lights;
};

SceneProperties parse(char * filename);
#endif
