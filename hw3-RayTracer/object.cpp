#include "object.h"

Sphere::Sphere(const vec3& _origin, const float& _radius) :
	origin(_origin), radius(_radius) {
}
bool Color::operator == (const Color &otherColor) {
    return r == otherColor.r && g == otherColor.g && b == otherColor.b;
}
Triangle::Triangle(const vec3& a, 
    const vec3& b, 
    const vec3& c,
    vec3 an, 
    vec3 bn, 
    vec3 cn) {
        vertexes[0] = a;
        vertexes[1] = b;
        vertexes[2] = c;
        vertexNormals[0] = an;
        vertexNormals[1] = bn;
        vertexNormals[2] = cn;
        type = triangle;
    };
