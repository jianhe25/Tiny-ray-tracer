// Transform.cpp: implementation of the Transform class.


#include "Transform.h"

//Please implement the following functions:
#include <stdio.h>
void debug(const mat3& mat) {
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j)
			printf("%f ",mat[i][j]);
		puts("");
	}
	puts("");
}
// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
  // YOUR CODE FOR HW1 HERE
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
	debug(result);
	return result;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
	vec3 y_axis(0.0, 1.0, 0.0);
	vec3 x_axis(1.0, 0.0, 0.0);
	eye = eye * rotate(degrees, y_axis);
	vec3 up_new = glm::cross(eye, x_axis);
	up = (up_new.y * up.y < 0)? -up_new : up_new;
  // YOUR CODE FOR HW1 HERE
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
	vec3 x_axis(1.0, 0.0, 0.0);
	eye = eye * rotate(degrees, x_axis);
	up = glm::cross(eye, x_axis);
  // YOUR CODE FOR HW1 HERE 
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
  // YOUR CODE FOR HW1 HERE
	//printf("eye = %f %f %f\n",eye.x, eye.y, eye.z);
	//printf("up = %f %f %f\n",up.x, up.y, up.z);

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
  // You will change this return call
	return translation * rotation;
}

vec3 Transform::GetUnit(const vec3& a) {
	float len = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
	return vec3(a.x / len, a.y / len, a.z / len);
}
Transform::Transform()
{

}

Transform::~Transform()
{

}
