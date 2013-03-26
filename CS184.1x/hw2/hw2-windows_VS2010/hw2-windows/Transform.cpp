// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
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

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
	eye = eye * rotate(degrees, up);
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
	vec3 ort = glm::cross(up,eye);

	eye = eye * rotate(-degrees, ort);
	up = up * rotate(-degrees, ort);
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
    float matrix2[16];

	vec3 c = center;
	vec3 f = glm::normalize(c - eye);
	vec3 u = glm::normalize(up);
	vec3 s = glm::normalize(glm::cross(f, u));

	u = glm::cross(s, f);

	mat4 m = mat4 (
		s.x, u.x, -f.x, 0.0 ,
		s.y, u.y, -f.y, 0.0 ,
		s.z, u.z, -f.z, 0.0 ,
		0.0, 0.0, 0.0, 1.0);

	return glm::transpose(glm::translate(m, -eye));
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	float f = 1.0 / tan(fovy/2);

	//TODO HERE
	mat4 ret (f/aspect, 0,        0,                         0,
			  0,        f,        0,                         0,
			  0,        0,        (zFar+zNear)/(zNear-zFar), 2*zFar*zNear/(zNear-zFar),
			  0,        0,        -1,                        0);

    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well.
	return glm::transpose(glm::perspective(fovy,aspect,zNear,zFar));
	return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
	mat4 ret(1);
    // YOUR CODE FOR HW2 HERE
    // Implement scaling
	ret[0][0]= sx;
	ret[1][1]= sy;
	ret[2][2]= sz;

	return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
	mat4 ret(1.0);

	ret[0][3] = tx;
	ret[1][3] = ty;
	ret[2][3] = tz;

    // YOUR CODE FOR HW2 HERE
    // Implement translation 
    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}