// Transform.cpp: implementation of the Transform class.


#include "Transform.h"
#include <stdio.h>

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
  // YOUR CODE FOR HW2 HERE
	float radian = degrees / 180.0 * pi;
	float x = axis.x, y = axis.y, z = axis.z;
	mat3 Identity;
	mat3 aa(x*x, x*y, x*z,
			x*y, y*y, y*z,
			x*z, y*z, z*z);
	mat3 Astar(0.0, -z, y,
			   z, 0.0, -x,
			   -y, x, 0.0);
	mat3 result = cos(radian) * Identity + (1.0 - cos(radian)) * aa +  sin(radian) * Astar;
	return result;  
}

void Transform::left(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW2 HERE
	vec3 y_axis(0.0, 1.0, 0.0);
	eye = eye * rotate(degrees, y_axis);
	up = up * rotate(degrees, y_axis);
}

void Transform::up(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW2 HERE 
	vec3 x_axis(1.0, 0.0, 0.0);
	eye = eye * rotate(degrees, x_axis);
	up = up * rotate(degrees, x_axis);
}

void debug(const vec3 &vec, const char* message="") {
	printf("%s: %.2f %.2f %.2f\n",message, vec.x, vec.y, vec.z);
}
mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) {
	//debug(eye, "eye");
	//debug(up, "up");
	//debug(center, "center");
  // YOUR CODE FOR HW2 HERE
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
}

/*
	My Notes about perpective, it takes me some time to understand what's going on there.
	the perspective matrix project the frustum formed by near plane, far plane, field of view (fovy, length of y), 
	aspect (length of x) to an 1*1*2 cuboid. the projection plane is defined by fovy, because y size of projection plane is 1,
	we can determine d, which is distance to origin, by fovy.
	(Actually, fovy not only decide the y size of frustum, also decide the projection plane, and aspect is another variable to 
	determine x size of frustum, it use relative ratio between x size and y size, and the result is x' = x / aspect)
*/
mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	float theta = fovy / 2.0 / 180.0 * pi;
	float d = 1.0 / glm::tan(theta);
	float A = -(zFar + zNear) / (zFar - zNear);
	float B = -2 * zFar * zNear / (zFar - zNear);
	mat4 perspective(d / aspect, 0.0, 0.0, 0.0,
					 0.0, d, 0.0, 0.0,
					 0.0, 0.0, A, B,
					 0.0, 0.0, -1.0, 0.0);
	return perspective;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) {
  // YOUR CODE FOR HW2 HERE
	return mat4(sx, 0.0, 0.0, 0.0,
			    0.0, sy, 0.0, 0.0,
				0.0, 0.0, sz, 0.0,
				0.0, 0.0, 0.0, 1.0);
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) {
  // YOUR CODE FOR HW2 HERE
	return mat4(1.0, 0.0, 0.0, tx,
				0.0, 1.0, 0.0, ty,
				0.0, 0.0, 1.0, tz,
				0.0, 0.0, 0.0, 1.0);
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) {
  vec3 x = glm::cross(up,zvec) ; 
  vec3 y = glm::cross(zvec,x) ; 
  vec3 ret = glm::normalize(y) ; 
  return ret ; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
