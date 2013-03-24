// Transform.cpp: implementation of the Transform class.


#include "Transform.h"

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
  // YOUR CODE FOR HW1 HERE

  // You will change this return call
  //return mat3();

	double phi = degrees/180 * pi;
    vec3 u = glm::normalize(axis);
 
    return mat3 (
            cos(phi) + u.x * u.x * (1 - cos(phi)),
            u.x * u.y * (1 - cos(phi)) - u.z * sin(phi),
            u.x * u.z * (1 - cos(phi)) + u.y * sin(phi),
 
            u.y * u.x * (1 - cos(phi)) + u.z * sin(phi),
            cos(phi) + u.y * u.y * (1 - cos(phi)),
            u.y * u.z * (1 - cos(phi)) - u.x * sin(phi),
               
            u.z * u.x * (1 - cos(phi)) - u.y * sin(phi),
            u.z * u.y * (1 - cos(phi)) + u.x * sin(phi),
            cos(phi) + u.z * u.z * (1 - cos(phi)));
}

#include <iostream>

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE
	eye = eye * rotate(degrees, up);
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE 
	vec3 ort = glm::cross(up, eye);
 	eye = eye * rotate(-degrees, ort);
    up = up * rotate(-degrees, ort);
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
  // YOUR CODE FOR HW1 HERE

  // You will change this return call
  //return mat4();
	
	vec3 w = glm::normalize(eye);
	vec3 u = glm::normalize(glm::cross(up, w));
	vec3 v = glm::cross(w, u);
	mat4 rotation(u.x, u.y, u.z, 0.0,
				  v.x, v.y, v.z, 0.0,
				  w.x, w.y, w.z, 0.0,
				  0.0, 0.0, 0.0, 1.0);
	mat4 translation(1.0, 0.0, 0.0, -eye.x,
					 0.0, 1.0, 0.0, -eye.y,
					 0.0, 0.0, 1.0, -eye.z,
					 0.0, 0.0, 0.0, 1.0);
	return translation * rotation;
	

	//return glm::transpose(glm::lookAt(eye,vec3(0,0,0),up));

/*	vec3 a = eye;
  vec3 b = up;
  
  vec3 w = glm::normalize(a);
  vec3 u = glm::normalize(glm::cross(b,w));
  vec3 v = glm::cross(w,u);
  
  //here we write the matrices in row major form 
  //because the main program automatically transpose 
  //the matrix
  mat4 R = mat4(
		  u.x, u.y, u.z, 0, 
		  v.x, v.y, v.z, 0,
		  w.x, w.y, w.z, 0,
		  0,   0,   0,   1 
	       );

  mat4 T = mat4(
		  1,   0,   0,	 -eye.x,
		  0,   1,   0,   -eye.y,
		  0,   0,   1,   -eye.z,
		  0,   0,   0,        1
	       );
  return  T * R;
  */

	/*mat4 translation(
    1, 0, 0, -eye[0],
    0, 1, 0, -eye[1],
    0, 0, 1, -eye[2],
    0, 0, 0, 1
  );

  vec3 w = glm::normalize(eye);
  vec3 u = glm::normalize(glm::cross(up, w));
  vec3 v = glm::normalize(glm::cross(w, u));
  mat4 rotation(
    u[0], u[1], u[2], 0,
    v[0], v[1], v[2], 0,
    w[0], w[1], w[2], 0,
    0, 0, 0, 1
  );


  return translation * rotation;

  // Similar result
  // Here just for learning
  mat4 result(
    u[0], u[1], u[2], glm::dot(u, -eye),
    v[0], v[1], v[2], glm::dot(v, -eye),
    w[0], w[1], w[2], glm::dot(w, -eye),
    0, 0, 0, 1
  );

  return result;
  */

	// YOUR CODE FOR HW1 HERE
  /*vec3 w=glm::normalize(eye);
  vec3 u=glm::normalize(glm::cross(up,w));
  vec3 v=glm::cross(w,u);

  float wx=w.x;float wy=w.y;float wz=w.z;
  float ux=u.x;float uy=u.y;float uz=u.z;
  float vx=v.x;float vy=v.y;float vz=v.z;

  vec3 neye=glm::normalize(eye);
  float eyex=eye.x;float eyey=eye.y;float eyez=eye.z;
 // printf("\nCoordinates eye: x: %.2f, y: %.2f, z: %.2f; distance: %.2f\n", 
	// neye.x, neye.y, neye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));

  // mat4 lookAtMatrix=mat4(ux,vx,wx,0,uy,vy,wy,0,uz,vz,wz,0,-ux*eyex-uy*eyey-uz*eyez,-vx*eyex-vy*eyey-vz*eyez,wx*eyex-wy*eyey-wz*eyez,1);
  // mat4 lookAtMatrix=mat4(ux,vx,wx,0,uy,vy,wy,0,uz,vz,wz,0,
  // 	-ux*eyex-uy*eyey-uz*eyez,
  // 	-vx*eyex-vy*eyey-vz*eyez,
  // 	wx*eyex-wy*eyey-wz*eyez,1);


mat4 wuv=mat4(ux,vx,wx,0,  uy,vy,wy,0,  uz,vz,wz,0,  0,0,0,1);
mat4 trMat=mat4(1,0,0,0,  0,1,0,0,  0,0,1,0, -eyex,-eyey,-eyez,1);
mat4 lookAtMatrix=glm::transpose(wuv*trMat);
  // mat4 lookAtMatrix=mat4(ux,uy,uz,-ux*eyex-uy*eyey-uz*eyez,vx,vy,vz,
  	// -vx*eyex-vy*eyey-vz*eyez,wx,wy,wz,wx*eyex-wy*eyey-wz*eyez,0,0,0,1);
 printf("\n\n\nCoordinates w: x: %.2f, y: %.2f, z: %.2f;",w.x,w.y,w.z);
 printf("\nCoordinates u: x: %.2f, y: %.2f, z: %.2f;",u.x,u.y,u.z);
 printf("\nCoordinates v: x: %.2f, y: %.2f, z: %.2f;",v.x,v.y,v.z);

  // You will change this return call
  return lookAtMatrix;
  */
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
