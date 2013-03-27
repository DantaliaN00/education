#include <iostream>

#include "Box.h"
#include "Ray.h"
#include "geometry.h"
#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

bool sphereTest1(){
  SceneObject* sphere1 = new SceneObject();
  sphere1->type = SPHERE;
  sphere1->center = vec3(1.0f,1.0f,1.0f);
  sphere1->radius = 1.0f;
  sphere1->transform = mat4(1);
  sphere1->invTrans = mat4(1);
  Box box1 = Box(sphere1);
  return 
    (box1.minPoint == vec3(0, 0, 0)) &&
    (box1.maxPoint == vec3(2, 2, 2));
}

bool sphereTest2(){
  SceneObject sphere = SceneObject();
  sphere.type = SPHERE;
  sphere.center = vec3(1,0,0);
  sphere.radius = 1;
  sphere.transform = mat4(1);
  sphere.invTrans = mat4(1);
  Box box = Box(&sphere);
  return 
    (box.minPoint == vec3(0, -1, -1)) &&
    (box.maxPoint == vec3(2, 1, 1));
}

bool triTest1(){
  SceneObject tri = SceneObject();
  //tri
  tri.type = TRI;
  tri.v1 = vec3(0);
  tri.v2 = vec3(1,0,0);
  tri.v3 = vec3(1);
  Box box = Box(&tri);
  return 
    (box.minPoint == vec3(0)) &&
    (box.maxPoint == vec3(1));
}

bool boxIntersectTest1(){
  Ray r;
  r.origin = vec3(-1,0,0);
  r.direction = vec3(1,0,0);

  SceneObject s;
  s.type = SPHERE;
  s.center = vec3(1,0,0);
  s.radius = 1;
  s.transform = mat4(1);
  s.invTrans = mat4(1);
  
  Box b = Box(&s);
  return b.intersectBox(r) == 1.0;
}

bool boxIntersectTest2(){
  // Failing a boundry condition

  Ray r;
  r.origin = vec3(-1,0,0);
  r.direction = vec3(1,0,0);

  SceneObject s;
  s.type = SPHERE;
  s.center = vec3(1,1,1);
  s.radius = 1;
  s.transform = mat4(1);
  s.invTrans = mat4(1);
  
  Box b = Box(&s);
  return b.intersectBox(r) > 0;
}

bool boxConstructorTest1(){
  SceneObject* sphere1 = new SceneObject();
  sphere1->type = SPHERE;
  sphere1->center = vec3(1.0f,1.0f,1.0f);
  sphere1->radius = 1.0f;
  sphere1->transform = mat4(1);
  sphere1->invTrans = mat4(1);
  Box box1 = Box(sphere1);

  Box box2 = Box(&box1);
  return
    box2.minPoint == box1.minPoint &&
    box1.maxPoint == box2.maxPoint &&
    box1.lChild == box2.lChild &&
    box1.rChild == box2.rChild;
}

bool boxConstructorTest2(){
  SceneObject sphere1 = SceneObject();
  sphere1.type = SPHERE;
  sphere1.center = vec3(1.0f,1.0f,1.0f);
  sphere1.radius = 1.0f;
  sphere1.transform = mat4(1);
  sphere1.invTrans = mat4(1);

  SceneObject sphere2 = SceneObject();
  sphere2.type = SPHERE;
  sphere2.center = vec3(-1.0f,-1.0f,-1.0f);
  sphere2.radius = 1.0f;
  sphere2.transform = mat4(1);
  sphere2.invTrans = mat4(1);

  Box *box1 = new Box(&sphere1);
  Box *box2 = new Box(&sphere2);
  Box *box3 = new Box(box1, box2);
  return 
    box3->lChild == box1 && box3->rChild == box2 &&
    box3->minPoint == vec3(-2,-2,-2) &&
    box3->maxPoint == vec3(2,2,2);
}

bool boxConstructorTest3(){
  SceneObject sphere1 = SceneObject();
  sphere1.type = SPHERE;
  sphere1.center = vec3(1.0f,1.0f,1.0f);
  sphere1.radius = 1.0f;
  sphere1.transform = mat4(1);
  sphere1.invTrans = mat4(1);

  SceneObject sphere2 = SceneObject();
  sphere2.type = SPHERE;
  sphere2.center = vec3(-1.0f,-1.0f,-1.0f);
  sphere2.radius = 1.0f;
  sphere2.transform = mat4(1);
  sphere2.invTrans = mat4(1);

  Box *box1 = new Box(&sphere1);
  Box *box2 = new Box(&sphere2);
  Box *box3 = new Box(box1, box2);
  
  delete box3;
  return true;
}

bool boxConstructorTest4(){
  SceneObject sphere1 = SceneObject();
  sphere1.type = SPHERE;
  sphere1.center = vec3(1.0f,1.0f,1.0f);
  sphere1.radius = 1.0f;
  sphere1.transform = mat4(1);
  sphere1.invTrans = mat4(1);

  SceneObject sphere2 = SceneObject();
  sphere2.type = SPHERE;
  sphere2.center = vec3(-1.0f,-1.0f,-1.0f);
  sphere2.radius = 1.0f;
  sphere2.transform = mat4(1);
  sphere2.invTrans = mat4(1);

  SceneObject sphere3 = SceneObject();
  sphere3.type = SPHERE;
  sphere3.center = vec3(-1.0f,-1.0f,1.0f);
  sphere3.radius = 1.0f;
  sphere3.transform = mat4(1);
  sphere3.invTrans = mat4(1);
  
  vector<Box*> boxVec;
  boxVec.push_back(new Box(&sphere1));
  boxVec.push_back(new Box(&sphere2));
  boxVec.push_back(new Box(&sphere3));
  Box *b = new Box(&boxVec,0);
  delete b;
  return true;
}

bool testTree1(){
  SceneObject s1;
  s1.type = SPHERE;
  s1.center = vec3(1,1,1);
  s1.radius = 1;
  s1.transform = mat4(1);
  s1.invTrans = mat4(1);

  SceneObject s2;
  s2.type = SPHERE;
  s2.center = vec3(-1,-1,-1);
  s2.radius = 1;
  s2.transform = mat4(1);
  s2.invTrans = mat4(1);

  vector<SceneObject*> v;
  v.push_back(&s1);
  v.push_back(&s2);

  Box b = Box(&v);
  return 
    b.minPoint == vec3(-2,-2,-2) &&
    b.maxPoint == vec3(2,2,2);
}

bool testTree2(){
  SceneObject s1;
  s1.type = SPHERE;
  s1.center = vec3(1,1,1);
  s1.radius = 1;
  s1.transform = mat4(1);
  s1.invTrans = mat4(1);

  SceneObject s2;
  s2.type = SPHERE;
  s2.center = vec3(-1,-1,-1);
  s2.radius = 1;
  s2.transform = mat4(1);
  s2.invTrans = mat4(1);

  SceneObject s3;
  s3.type = SPHERE;
  s3.center = vec3(-3,-3,-3);
  s3.radius = 1;
  s3.transform = mat4(1);
  s3.invTrans = mat4(1);

  SceneObject s4;
  s4.type = SPHERE;
  s4.center = vec3(3,3,3);
  s4.radius = 1;
  s4.transform = mat4(1);
  s4.invTrans = mat4(1);

  vector<SceneObject*> v;
  v.push_back(&s1);
  v.push_back(&s2);
  v.push_back(&s3);
  v.push_back(&s4);

  Box b = Box(&v);
  return 
    b.minPoint == vec3(-4,-4,-4) &&
    b.maxPoint == vec3(4,4,4);  
}

bool testIntersectionTest1(){
  SceneObject s1;
  s1.type = SPHERE;
  s1.center = vec3(1,1,1);
  s1.radius = 1;
  s1.transform = mat4(1);
  s1.invTrans = mat4(1);

  SceneObject s2;
  s2.type = SPHERE;
  s2.center = vec3(-1,-1,-1);
  s2.radius = 1;
  s2.transform = mat4(1);
  s2.invTrans = mat4(1);

  SceneObject s3;
  s3.type = SPHERE;
  s3.center = vec3(-3,-3,-3);
  s3.radius = 1;
  s3.transform = mat4(1);
  s3.invTrans = mat4(1);

  SceneObject s4;
  s4.type = SPHERE;
  s4.center = vec3(3,3,3);
  s4.radius = 1;
  s4.transform = mat4(1);
  s4.invTrans = mat4(1);

  vector<SceneObject*> v;
  v.push_back(&s1);
  v.push_back(&s2);
  v.push_back(&s3);
  v.push_back(&s4);

  Box b = Box(&v);
  SceneObject* sTest = NULL;
  Ray r;
  r.origin = vec3(3,1,1);
  r.direction = vec3(-1,0,0);

  float testT = b.testIntersection(r, &sTest);
  return testT == 1.0 && sTest->center == s1.center;//sTest->type == SPHERE && 
    //    s1->center == vec3(1,1,1);
}

int main(){
  cout << (sphereTest1() ? "." : "E")
       << (sphereTest2() ? "." : "E")
       << (triTest1() ? "." : "E")
       << (boxConstructorTest1() ? "." : "E")
       << (boxConstructorTest2() ? "." : "E")
       << (boxConstructorTest3() ? "." : "E")
       << (boxConstructorTest4() ? "." : "E")
       << (boxIntersectTest1() ? "." : "E")
       << (boxIntersectTest2() ? "." : "E")
       << (testTree1() ? "." : "E")
       << (testTree2() ? "." : "E")
       << (testIntersectionTest1() ? "." : "E");
  cout << endl;

  return 0;
}
