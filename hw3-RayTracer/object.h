// Include the helper glm library, including matrix transform extensions

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#ifndef OBJECT_H
#define OBJECT_H

typedef glm::mat3 mat3 ;
typedef glm::mat4 mat4 ; 
typedef glm::vec3 vec3 ; 
typedef glm::vec4 vec4 ; 
typedef unsigned char BYTE;

struct Color
{
	float r, g, b;
	Color() : r(0), g(0), b(0) {}
	Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
    BYTE Rbyte() { return r * 255; }
	BYTE Gbyte() { return g * 255; }
	BYTE Bbyte() { return b * 255; }
	bool operator == (const Color &otherColor);
};

const Color BLACK(0, 0, 0);
const Color WHITE(255, 255, 255);


// Materials (read from file) 
// With multiple objects, these are colors for each.
struct Materials {
	Color diffuse; 
	Color specular;
	Color emission; 
	float shininess ;
};
//static enum {view, translate, scale} transop; // which operation to transform 
class Object {
public:
  	mat4 transform; 
  	Materials materials;	

    enum shape {triangle, sphere} ;
    shape type; 
    
	Object() {}
	virtual void dumy(){};
};

class Sphere : public Object {
public:
	vec3 origin;
	float radius;
	Sphere(const vec3& _origin, const float& _radius);
};

class Triangle : public Object {
public:
    vec3 vertexes[3], vertexNormals[3];
	Triangle(const vec3& a, 
		const vec3& b, 
		const vec3& c,
		vec3 an = vec3(0,0,0), 
		vec3 bn = vec3(0,0,0), 
		vec3 cn = vec3(0,0,0));
    Triangle() {}
};
#endif // _OBJECT_H
