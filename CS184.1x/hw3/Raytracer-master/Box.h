#ifndef BOX_H
#define BOX_H

#include "Ray.h"
#include "geometry.h"
#include <vector>

class Box {
 private:
  void join( Box* left, Box* right);
  void tree( vector<Box*> *shapeVec, int axis );
  void nullBox();
  void copy(Box* b);

 public:
  vec3 minPoint, maxPoint;
  SceneObject *myShape;
  Box *lChild, *rChild;
  
  Box ();
  Box( Box * );
  Box ( SceneObject *obj );
  Box ( vector<SceneObject*> *shapeVec );
  Box ( Box* left, Box* right );
  Box ( vector<Box*> *shapeVec, int axis );
  
  //~Box();
  
  float intersectBox(Ray r);
  float intersectShape(Ray r);
  float testIntersection(Ray r, SceneObject** s);
};
#endif
