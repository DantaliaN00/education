#include "Box.h"
#include <glm/glm.hpp>
#include <cfloat>
#include "geometry.h"
#include <iostream>

Box::Box( SceneObject *obj ){
  lChild = NULL;
  rChild = NULL;
  myShape = obj;
  if( obj->type == SPHERE ){
    maxPoint = obj->center + vec3(obj->radius);
    minPoint = obj->center - vec3(obj->radius);
    maxPoint = vec3(obj->transform * vec4(maxPoint,1.0f));
    minPoint = vec3(obj->transform * vec4(minPoint,1.0f));
  } else if( obj->type == TRI || obj->type == TRINORMAL ){
    maxPoint = max(myShape->v1, max(myShape->v2, myShape->v3));
    minPoint = min(myShape->v1, max(myShape->v2, myShape->v3));
    
      /*vec3(min(myShape.v1[0],min(myShape.v2[0],myShape.v3[0])),
                    min(myShape.v1[1],min(myShape.v2[1],myShape.v3[1])),
                    min(myShape.v1[2],min(myShape.v2[2],myShape.v3[2])));*/

  } else {
    // Error: this should never happen
    minPoint = vec3(0);
    maxPoint = vec3(0);
  }
}

void Box::nullBox(){
  minPoint = vec3(0);
  maxPoint = vec3(0);
  /*
  myShape->type = SPHERE;
  myShape->center = vec3(0);
  myShape->radius = 0;
  */
  myShape = NULL;
  lChild = NULL;
  rChild = NULL;
}

Box::Box(){
  nullBox();
}

void Box::copy(Box *b){
  minPoint = b->minPoint;
  maxPoint = b->maxPoint;
  myShape = b->myShape;
  if( b->lChild != NULL ){
    lChild = new Box(b->lChild);
  } else {
    lChild = NULL;
  }
  if( b->rChild != NULL ){
    rChild = new Box(b->rChild);
  } else {
    rChild = NULL;
  }
}

Box::Box(Box* b){
  copy(b);
}

void Box::join(Box* left, Box* right){
  myShape = NULL;
  if( left != NULL && right != NULL ){
    lChild = left;
    rChild = right;
    minPoint = min(left->minPoint, right->minPoint);
    maxPoint = max(left->maxPoint, right->maxPoint);
  } else if( left != NULL ){
    lChild = left;
    rChild = NULL;
    minPoint = left->minPoint;
    maxPoint = left->maxPoint;
  } else if( right != NULL ){
    lChild = right;
    rChild = NULL;
    minPoint = right->minPoint;
    maxPoint = right->maxPoint;
  } else {
    Box();
    // lChild = NULL;
    // rChild = NULL;
    // minPoint = vec3(0);
    // maxPoint = vec3(0);
  }    
}

Box::Box( Box* left, Box* right ){
  join(left, right);
}

Box::Box(vector<SceneObject*> *shapeVec){
  vector<Box*> boxVec;
  Box* b;
  vec3 
    minVec = vec3(FLT_MAX),
    maxVec = vec3(FLT_MIN);
  
  for(vector<SceneObject*>::iterator it = shapeVec->begin();
      it < shapeVec->end(); it++){
    b = new Box(*it);
    minVec = min(minVec, b->minPoint);
    maxVec = max(maxVec, b->maxPoint);
    boxVec.push_back(b);
  }
  vec3 diff = maxVec - minVec;
  // Chose the longest axis
  if( diff[0] > diff[1] && diff[0] > diff[2] ){
    tree( &boxVec, 0 );
  } else if( diff[1] > diff[0] && diff[1] > diff[2] ){
    tree( &boxVec, 1 );
  } else {
    tree( &boxVec, 2 );
  }
}

void Box::tree( vector<Box*> *shapeVec, int axis ){
    axis %= 3; // Axis is in the range 0,1,2, for x,y,z
  if( shapeVec->size() == 0 ){
    nullBox();
  } else if( shapeVec->size() == 1 ){
    copy(shapeVec->at(0));
    //delete shapeVec[0];
  } else if( shapeVec->size() == 2 ){
    join( new Box(shapeVec->at(0)), new Box(shapeVec->at(1)) );
    //delete shapeVec[0];
    //delete shapeVec[1];
  } else {
    vec3 sumVec = vec3(0);
    vector<Box*>::iterator it;
    for( it = shapeVec->begin(); it < shapeVec->end(); it++ ){
      sumVec = sumVec + (*it)->maxPoint - (*it)->minPoint;
    }
    vec3 meanVec = sumVec/vec3(shapeVec->size());
    vector<Box*> less, more;
    for( it = shapeVec->begin(); it < shapeVec->end(); it++ ){
      vec3 boxSum = (*it)->maxPoint - (*it)->minPoint;
      if( boxSum[axis] < meanVec[axis] ){
        less.push_back(*it);
      } else {
        more.push_back(*it);
      }
    }
    
    // Check this part of the algorithm
    if( less.size() == 0 ){
      while( less.size() < more.size() ){
        less.push_back( new Box(more.back()) );
        more.pop_back();
      }
    }
    join( new Box(&less, axis+1), new Box(&more, axis+1) );
  }
}

Box::Box(vector<Box*> *shapeVec, int axis){
  tree(shapeVec, axis);
}

/*
Box::~Box(){
  if( lChild != NULL ){
    delete lChild;
    lChild = NULL;
  }
  if( rChild != NULL ){
    delete rChild;
    rChild = NULL;
  }
}
*/

float Box::intersectBox(Ray r){

  // Find t ranges;
  vec3 
    p1 = (minPoint - r.origin)/r.direction,
    p2 = (maxPoint - r.origin)/r.direction;
  
  vec3 
    minP = min(p1,p2),
    maxP = max(p1,p2);

  float 
    minT = glm::max(minP[0], glm::max(minP[1], minP[2])),
    maxT = glm::min(maxP[0], glm::min(maxP[1], maxP[2]));

  if( (maxT < minT) || (maxT < 0) ){
    return -1.0;
  }
  return minT > 0 ? minT : 0.0;
}

float Box::intersectShape(Ray r){
  return myShape->testIntersection(r);
}

float Box::testIntersection(Ray r, SceneObject **s){
  if( intersectBox(r) < 0.0 ){
    return -1.0;
  }

  if( lChild != NULL && rChild != NULL ){
    SceneObject *s1 = NULL, *s2 = NULL;
    float 
      tVal1 = lChild->testIntersection(r, &s1),
      tVal2 = rChild->testIntersection(r, &s2);
    
    // Return the smallest t >= 0
    if( tVal1 < 0.0 ){
      *s = s2;
      return tVal2;
    } else if( tVal2 < 0.0 ){
      *s = s1;
      return tVal1;
    } else {
      if( tVal1 < tVal2 ){
        *s = s1;
        return tVal1;
      } else {
        *s = s2;
        return tVal2;
      }
    }
  } else if( lChild != NULL ){
    return lChild->testIntersection(r, s);
  } else {
    *s = myShape;
    return myShape->testIntersection(r);
  }
}


