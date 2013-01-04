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

void debug(const vec3 &vec, const char* message="") {
	printf("%s: %.2f %.2f %.2f\n",message, vec.x, vec.y, vec.z);
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

Transform::Transform()
{

}

Transform::~Transform()
{

}
