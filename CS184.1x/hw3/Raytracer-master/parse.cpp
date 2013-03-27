#include "parse.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Box.h"

#define MAX_ARGS 100

using namespace std;
using namespace glm;

// assign increasing counter to scene objects
int counter = 0;
int setCounter() {
    counter++;
    return counter;
}
    
extern bool debugParsing;


glm::vec3 curAmbient;
glm::vec3 curDiffuse;
glm::vec3 curSpecular;
glm::vec3 curEmission;
float curShininess;
float curAlpha = 1.0;


//set some sane defaults
SceneProperties::SceneProperties() {
    maxdepth = 5;
    maxverts = 1000;
    maxvertnorms = 1000;
    height = 100;
    width = 100;
    backgroundColor = vec3(0,0,0);
}

SceneProperties::~SceneProperties() {

}

void SceneProperties::insertObject(SceneObject obj) {
    this->objects.push_back(obj);
}



SceneProperties parse(char * filename) {
    //sane defaults
    curAmbient = vec3(0.2,0.2,0.2);
    
    curDiffuse = vec3(1,1,1);
    curSpecular = vec3(0,0,0);
    curEmission = vec3(0,0,0);
    curShininess = 10.0;

    bool cameraSpecified = false;

    //open file

    ifstream file;
    file.open(filename);
    if (file.fail()) {
        cout << "Could not open file, exiting\n";
        exit(2);
    }

    //set up scene to return
    SceneProperties scene;

    //establish variable for current transform
    //initialized to identity
    //Enable transformations
    vector<mat4> transformStack;
    mat4 transformMatrix(1.0f);

    //loop over lines
    string line;
    int i = 0;
    while (!file.eof()) {
        getline(file,line);

        //ignore comments
        if (line[0] == '#') {
            continue;
        }

        else if (line.size() == 0) {
            continue;
        }

        else {
            //convert to c string
            char cstring[(int) line.size()]; 
            strcpy(cstring, line.c_str());

            char * tokenPointer;
            tokenPointer = strtok(cstring, " \t"); //delimits with either space or tab
            if (debugParsing)
                cout << "Parsing line: ";


            string cmds[100];
            int idx = 0;
            while (tokenPointer != NULL) {
                if (debugParsing)
                    printf("%s ", tokenPointer);
                cmds[idx] = string(tokenPointer);
                idx++;
                tokenPointer = strtok(NULL, " \t");
            }

            //and parcel out the scene properties

            if (cmds[0] == "size") {
                scene.width = atoi(cmds[1].c_str());
                scene.height = atoi(cmds[2].c_str());
            }

            else if (cmds[0] == "maxdepth") {
                scene.maxdepth = atoi(cmds[1].c_str());
            }

            else if (cmds[0] == "output") {
                scene.output = cmds[1].c_str();
            }

            else if (cmds[0] == "maxverts") {
                scene.maxverts = atoi(cmds[1].c_str());
            }

            else if (cmds[0] == "maxvertnorms") {
                scene.maxvertnorms = atoi(cmds[1].c_str());
            }

            else if (cmds[0] == "camera") {

                cameraSpecified = true;
                if (idx < 11) {
                    cerr << "Improper number of camera parameters , exiting\n";
                    exit(10);
                }

                scene.cameraParams.lookFromX = atof(cmds[1].c_str());
                scene.cameraParams.lookFromY = atof(cmds[2].c_str());
                scene.cameraParams.lookFromZ = atof(cmds[3].c_str());

                scene.cameraParams.lookAtX = atof(cmds[4].c_str());
                scene.cameraParams.lookAtY = atof(cmds[5].c_str());
                scene.cameraParams.lookAtZ = atof(cmds[6].c_str());

                scene.cameraParams.upX = atof(cmds[7].c_str());
                scene.cameraParams.upY = atof(cmds[8].c_str());
                scene.cameraParams.upZ = atof(cmds[9].c_str());

                scene.cameraParams.fov = atof(cmds[10].c_str());
            }

            else if (cmds[0] == "sphere") {
                SceneObject s;
                s.id = setCounter();
                s.type = SPHERE;
                s.center.x = atof(cmds[1].c_str());
                s.center.y = atof(cmds[2].c_str());
                s.center.z = atof(cmds[3].c_str());
                s.radius = atof(cmds[4].c_str());
                s.transform = mat4(transformMatrix);

                s.ambient = curAmbient;
                s.emission = curEmission;
                s.diffuse = curDiffuse;
                s.specular = curSpecular;
                s.shininess = curShininess;
                s.alpha = curAlpha;

                s.invTrans = mat4(inverse(transformMatrix));
                //scene.objects.push_back(s);
                scene.insertObject(s);
            }

            else if (cmds[0] == "glasssphere") {
                SceneObject s;
                s.id = setCounter();
                s.type = SPHERE;
                s.center.x = atof(cmds[1].c_str());
                s.center.y = atof(cmds[2].c_str());
                s.center.z = atof(cmds[3].c_str());
                s.radius = atof(cmds[4].c_str());
                s.transform = mat4(transformMatrix);

                s.ambient = curAmbient;
                s.emission = curEmission;
                s.diffuse = curDiffuse;
                s.specular = curSpecular;
                s.shininess = curShininess;

                s.invTrans = mat4(inverse(transformMatrix));
                //scene.objects.push_back(s);
                scene.insertObject(s);


            }

            else if (cmds[0] == "vertex") {
                if (scene.vertices.size() > scene.maxverts) {
                    continue;
                }
                vec3 v(atof(cmds[1].c_str()), atof(cmds[2].c_str()), atof(cmds[3].c_str()));
                scene.vertices.push_back(v);
            } 

            else if (cmds[0] == "vertexnormal") {
                if (scene.normalvertices.size() > scene.maxvertnorms) {
                    continue;
                }

                VertexNormal vn;
                vn.position = vec3(atof(cmds[1].c_str()),
                                   atof(cmds[2].c_str()),
                                   atof(cmds[3].c_str()));

                vn.direction = vec3(atof(cmds[4].c_str()),
                                    atof(cmds[5].c_str()),
                                    atof(cmds[6].c_str()));

                scene.normalvertices.push_back(vn);

            }
            
            else if (cmds[0] == "tri") {
              SceneObject s;
              s.id = setCounter();
              s.type = TRI;
                s.ambient = curAmbient;
                s.emission = curEmission;
                s.diffuse = curDiffuse;
                s.specular = curSpecular;
                s.shininess = curShininess;
                s.alpha = curAlpha;
              s.v1 = vec3(transformMatrix*
                          vec4(scene.vertices[atoi(cmds[1].c_str())], 1.0f));
              s.v2 = vec3(transformMatrix*
                          vec4(scene.vertices[atoi(cmds[2].c_str())], 1.0f));
              s.v3 = vec3(transformMatrix*
                          vec4(scene.vertices[atoi(cmds[3].c_str())], 1.0f));
              s.transform = mat4(1.0);
              s.invTrans = mat4(1.0);
              //scene.objects.push_back(s);
              scene.insertObject(s);
              
            }

            else if (cmds[0] == "trinormal") {
              SceneObject s;
              s.id = setCounter();
              s.type = TRINORMAL;
              
                s.ambient = curAmbient;
                s.emission = curEmission;
                s.diffuse = curDiffuse;
                s.specular = curSpecular;
                s.shininess = curShininess;
                s.alpha = curAlpha;
              s.v1 = vec3(transformMatrix*
                          vec4(scene.normalvertices[atoi(cmds[1].c_str())].position, 1.0f));
              s.n1 = vec3(transformMatrix*
                          vec4(scene.normalvertices[atoi(cmds[1].c_str())].direction, 1.0f));
              s.v2 = vec3(transformMatrix*
                          vec4(scene.normalvertices[atoi(cmds[2].c_str())].position, 1.0f));
              s.n2 = vec3(transformMatrix*
                          vec4(scene.normalvertices[atoi(cmds[2].c_str())].direction, 1.0f));
              s.v3 = vec3(transformMatrix*
                          vec4(scene.normalvertices[atoi(cmds[3].c_str())].position, 1.0f));
              s.n3 = vec3(transformMatrix*
                          vec4(scene.normalvertices[atoi(cmds[3].c_str())].direction, 1.0f));

              //scene.objects.push_back(s);
              scene.insertObject(s);

            }

            //transformations
            else if (cmds[0] == "translate") {
              transformMatrix = glm::translate(transformMatrix, 
                                                vec3(atof(cmds[1].c_str()), 
                                                     atof(cmds[2].c_str()), 
                                                     atof(cmds[3].c_str())));

            }

            else if (cmds[0] == "rotate") {
              transformMatrix = glm::rotate(transformMatrix, 
                                             (float) atof(cmds[4].c_str()), 
                                             vec3(atof(cmds[1].c_str()), 
                                                  atof(cmds[2].c_str()), 
                                                  atof(cmds[3].c_str())));
              
            }

            else if (cmds[0] == "scale") {
                transformMatrix = glm::scale(transformMatrix, 
                        vec3(atof(cmds[1].c_str()), 
                             atof(cmds[2].c_str()), 
                             atof(cmds[3].c_str())));

            }

            else if (cmds[0] == "pushTransform") {
              //n.b. make this lowercase to avoid problems
              transformStack.push_back(transformMatrix);

            } 

            else if (cmds[0] == "popTransform") {
              transformMatrix = transformStack.back();
              transformStack.pop_back();

            }

            //lights
            else if (cmds[0] == "directional") {
                Light l;
                l.directional = true;

                l.location.x = atof(cmds[1].c_str());
                l.location.y = atof(cmds[2].c_str());
                l.location.z = atof(cmds[3].c_str());

                l.color.r = atof(cmds[4].c_str());
                l.color.g = atof(cmds[5].c_str());
                l.color.b = atof(cmds[6].c_str());

                scene.lights.push_back(l);

            }

            else if (cmds[0] == "point") {
                Light l;
                l.directional = false;
                l.location.x = atof(cmds[1].c_str());
                l.location.y = atof(cmds[2].c_str());
                l.location.z = atof(cmds[3].c_str());

                l.color.r = atof(cmds[4].c_str());
                l.color.g = atof(cmds[5].c_str());
                l.color.b = atof(cmds[6].c_str());

                scene.lights.push_back(l);

            }

            else if (cmds[0] == "attenuation") {
                //sets the attenuation of the light currently on the stack
                //if there is no such light, do nothing

                if (scene.lights.size() > 0) {
                    Light l = scene.lights.back();
                    l.constAttenuation = atof(cmds[1].c_str());
                    l.linearAttentuation = atof(cmds[2].c_str());
                    l.quadAttenuation = atof(cmds[3].c_str());
                    scene.lights.pop_back();
                    scene.lights.push_back(l);
                }

            }

            else if (cmds[0] == "ambient") {
                curAmbient = glm::vec3(atof(cmds[1].c_str()),
                                            atof(cmds[2].c_str()),
                                            atof(cmds[3].c_str())
                                            );
            }

            //material commands
            else if (cmds[0] == "diffuse") {
                curDiffuse = glm::vec3(atof(cmds[1].c_str()),
                                            atof(cmds[2].c_str()),
                                            atof(cmds[3].c_str())
                                            );
            }

            else if (cmds[0] == "specular") {
                curSpecular = glm::vec3(atof(cmds[1].c_str()),
                                            atof(cmds[2].c_str()),
                                            atof(cmds[3].c_str())
                                            );
            }
            else if (cmds[0] == "shininess") {
                curShininess = atof(cmds[1].c_str());
            }
            else if (cmds[0] == "emission") {
                curEmission = glm::vec3(atof(cmds[1].c_str()),
                                             atof(cmds[2].c_str()),
                                             atof(cmds[3].c_str())
                                             );
            }

            else if (cmds[0] == "alpha") {
                curAlpha = atof(cmds[1].c_str());
            }

            else if (cmds[0] == "bkgcolor") {
                scene.backgroundColor = vec3(atof(cmds[1].c_str()), atof(cmds[2].c_str()), atof(cmds[3].c_str()));
            }

            if (debugParsing)
                cout << " ...done with line\n";
        }
    }

    if (!cameraSpecified) {
        cerr << "No camera position specified, exiting\n";
        exit(10);
    }
    return scene;
}
