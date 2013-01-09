// Include the helper glm library, including matrix transform extensions

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "config.h"

#ifndef _OBJECT_H
#define _OBJECT_H

typedef glm::mat3 mat3 ;
typedef glm::mat4 mat4 ; 
typedef glm::vec3 vec3 ; 
typedef glm::vec4 vec4 ; 
typedef unsigned char BYTE;

extern const float eps;
int sgn(float x);

struct Ray {
    vec3 o;
    vec3 direction;
    Ray(const vec3& _o, const vec3& _direction);
};

struct Color
{
	float r, g, b;
	Color() : r(0), g(0), b(0) {}
	Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
    BYTE Rbyte() { return r * 255; }
	BYTE Gbyte() { return g * 255; }
	BYTE Bbyte() { return b * 255; }
	bool operator == (const Color &otherColor);
	Color operator * (const Color& otherColor);
};

const Color BLACK(0, 0, 0);
const Color WHITE(1.0, 1.0, 1.0);


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
	virtual ~Object();
	virtual bool Intersect(const Ray& ray, float* dis_to_ray);
	virtual vec3 InterpolatePointNormal(const vec3& point);
};

class Sphere : public Object {
public:
	vec3 o;
	float r;
	Sphere(const vec3& _o, const float& _r);
	
	virtual ~Sphere();
	virtual bool Intersect(const Ray& ray, float* dis_to_ray);
	virtual vec3 InterpolatePointNormal(const vec3& point);
};

class Triangle : public Object {
public:
    vec3 vertexes[3], vertexNormals[3];
    vec3 &a, &b, &c;
    vec3 &na, &nb, &nc;
    
	Triangle(const vec3& _a, 
		const vec3& _b, 
		const vec3& _c,
		vec3 na = vec3(0,0,0), 
		vec3 nb = vec3(0,0,0), 
		vec3 nc = vec3(0,0,0));
    
    virtual ~Triangle();
    virtual bool Intersect(const Ray& ray, float* dis_to_ray);
    virtual vec3 InterpolatePointNormal(const vec3& point);
};
#endif // _OBJECT_H
