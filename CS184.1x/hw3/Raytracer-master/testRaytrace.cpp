#include "raytrace.cpp"

using namespace glm;
using namespace std;

SceneProperties scene;
bool debug = false;
bool debugSingleRay = false;

void testGenerateRay(){

  // Basic test
  scene.width = 100;
  scene.height = 100;

  scene.maxdepth = 5;

  scene.output = "testOut.tga";
  
  scene.maxverts = 200;
  scene.maxvertnorms = 200;
   
  scene.cameraParams.lookFromX = 0.0;
  scene.cameraParams.lookFromY = 0.0;
  scene.cameraParams.lookFromZ = 1.0;

  scene.cameraParams.lookAtX = 0.0;
  scene.cameraParams.lookAtY = 0.0;
  scene.cameraParams.lookAtZ = 0.0;
  
  
  scene.cameraParams.upX = 0.0;
  scene.cameraParams.upY = 1.0;
  scene.cameraParams.upZ = 0.0;
  
  scene.cameraParams.fov = 90;
  
}

void testNormals(){
  
}

void 

int main(){
  testGenerateRay();
  testNormals();
  
  return 0; 
}
