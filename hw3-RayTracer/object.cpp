#include "object.h"
#include <algorithm>
#include <iostream>
#include <stdio.h>

const float eps = 1e-6;
int sgn(float x) { return x > eps? 1 : x < -eps? -1 : 0; }
bool IsSameVector(const vec3& a, const vec3& b) {
    return glm::dot(b-a, b-a) < eps;
}

bool Color::operator == (const Color &otherColor) const {
    return r == otherColor.r && g == otherColor.g && b == otherColor.b;
}
Color Color::operator * (const Color& otherColor) const {
    return Color(r * otherColor.r, g * otherColor.g, b * otherColor.b);
}
Color Color::operator + (const Color& otherColor) const {
    return Color(r + otherColor.r, g + otherColor.g, b + otherColor.b);
}
Color Color::operator * (const float scale) const {
    return Color(r * scale, g * scale, b * scale);
}


Ray::Ray(const vec3& _o, const vec3& _direction) :
    o(_o), direction(_direction) {
}

Object::Object() {
    //std::cerr << "The constructor of abstract object should not be called" << std::endl;
    //throw 2;
}
bool Object::Intersect(const Ray& ray, float* dis_to_ray) const {
    std::cerr << "Ray should not intersect with abstract object" << std::endl;
    throw 2; // This should never happen
}

vec3 Object::InterpolatePointNormal(const vec3& point) const {
    std::cerr << "Can't interpolate normal in abstract object" << std::endl;
    throw 2;
}

Sphere::Sphere(const vec3& _o, const float& _r) :
	o(_o), r(_r) {
    type = sphere;
}
double dot(vec3 a, vec3 b) {
    return (double)a.x*b.x + (double)a.y*b.y + (double)a.z*b.z;
}
bool Sphere::Intersect(const Ray& ray, float* dis_to_ray) const {
    //dir^2 * t^2 + 2*dir*(p-o)*t + (p-o)^2 == r*r;
    const vec3& dir = ray.direction;
    const vec3& p = ray.o;
    double c2 = glm::dot(dir, dir);
    double c1 = 2 * glm::dot(dir, p-o);
    double c0 = glm::dot(p-o, p-o) - r*r;
    double delta = c1*c1 - 4*c2*c0;
    if (delta < -1e-6) {
        return false;
    }
    
    delta = abs(delta);
    // closest intersection point
    double x = std::min((-c1 - sqrt(delta)) / (2*c2), 
                   (-c1 + sqrt(delta)) / (2*c2));
    
    if (x < -eps) {
        return false;
    } else {
        *dis_to_ray = x;
        return true;
    }
}

vec3 Sphere::InterpolatePointNormal(const vec3& point) const {
    //printf("distance = %f\n",glm::length(point - this->o) - this->r);
    assert(fabs(glm::length(point - this->o) - this->r) < 1); // ensure point on the surface
    return point - this->o;
}

Triangle::Triangle(
    const vec3& _a, 
    const vec3& _b, 
    const vec3& _c,
    vec3 _na, 
    vec3 _nb, 
    vec3 _nc) : 
    a(vertexes[0]),
    b(vertexes[1]),
    c(vertexes[2]),
    na(vertexNormals[0]),
    nb(vertexNormals[1]),
    nc(vertexNormals[2]) {
        a = _a;
        b = _b;
        c = _c;
        type = triangle;
        // no specified normal
        if (_na == vec3(0,0,0)) {
            na = nb = nc = glm::cross(b-a, c-a);
        } else {
            na = _na;
            nb = _nb;
            nc = _nc;
        }
    };

bool Triangle::Intersect(const Ray& ray, float* dis_to_ray) const {
    vec3 n = glm::normalize(glm::cross(b-a, c-a));
    const vec3& p = ray.o;
    const vec3& dir = ray.direction;
    
    // Ray parallel with plane
    if (sgn(glm::dot(dir, n)) == 0) {
        return false;
    }
    
    //plane ray intersection -- (p+dir*t)*n = a*n;
    float t = (glm::dot(a,n) - glm::dot(p,n)) / glm::dot(dir,n);
    vec3 p0 = p + (dir * t);
    
    // check if p0 in triangle, because triangle is convex.
    // if p0 in triangle, then absolute area equals to area_vec length, otherwise not.
    vec3 area_vec(0,0,0);
    float abs_area = 0.0;
    for (int i = 0; i < 3; ++i) {
        abs_area += glm::length(glm::cross(p0-vertexes[i], p0-vertexes[(i+1)%3]));
        area_vec += glm::cross(p0-vertexes[i], p0-vertexes[(i+1)%3]);
    }
    
    if (sgn(abs_area - glm::length(area_vec)) == 0) {
        *dis_to_ray = t;
        return true;
    } else
        return false;
}
vec3 Triangle::InterpolatePointNormal(const vec3& point) const {
    vec3 n = glm::cross(a-b, a-c);
    vec3 tmp_nb = glm::cross(c-point, a-point);
    vec3 tmp_nc = glm::cross(a-point, b-point);
    
    float beta = glm::dot(n, tmp_nb) / glm::dot(n,n);
    float gamma = glm::dot(n, tmp_nc) / glm::dot(n,n);
    float alpha = 1.0 - beta - gamma;
    return (na * alpha) + (nb * beta) + (nc * gamma);
}

Object::~Object() {
}
Triangle::~Triangle() {
}
Sphere::~Sphere() {
}
