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
bool Color::isZero() const { return Rbyte() == 0 && Gbyte() == 0 && Bbyte() == 0; }

Materials::Materials() : shininess(0.0) {}

Ray::Ray(const vec3& _o, const vec3& _direction) :
    o(_o), direction(_direction) {
}

Object::Object() : transform(1.0), InversedTransform(1.0) {
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
    float c2 = glm::dot(dir, dir);
    float c1 = 2 * glm::dot(dir, p-o);
    float c0 = glm::dot(p-o, p-o) - r*r;
    float delta = c1*c1 - 4*c2*c0;

    if (delta < -eps) {
        return false;
    }

    delta = fabs(delta);    
    // closest intersection point
    float x = std::min((-c1 - sqrt(delta)) / (2*c2), 
                   (-c1 + sqrt(delta)) / (2*c2));
    
    if (x < 1e-2) {
        if (x > 0) {
            //printf("sphere near zero = %f\n", x);
        }
        return false;
    } else {
        *dis_to_ray = x;
        return true;
    }
}
vec3 vec3TimeMat4(const vec3& a, const mat4& mat) {
    vec4 a_ex = vec4(a, 1.0f) * mat;
    return vec3(a_ex.x / a_ex.w, a_ex.y / a_ex.w, a_ex.z / a_ex.w);
}
vec3 ray3TimeMat4(const vec3& a, const mat4& mat) {
    return vec3(vec4(a, 0.0f) * mat);
}
vec3 Sphere::InterpolatePointNormal(const vec3& point) const {
    //printf("distance = %f\n",glm::length(point - this->o) - this->r);
    //assert(fabs(glm::length(point - this->o) - this->r) < 1); // ensure point on the surface
    vec3 p = vec3TimeMat4(point, this->InversedTransform);
    return vec3(vec4(p-o, 0.0f) * glm::transpose(this->InversedTransform));
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
    vec3 n = glm::cross(b-a, c-a);
    const vec3& p = ray.o;
    const vec3& dir = ray.direction;
    
    // Ray parallel with plane
    if (sgn(glm::dot(dir, n)) == 0) {
        return false;
    }
    
    //plane ray intersection -- (p+dir*t)*n = a*n;
    float t = (glm::dot(a,n) - glm::dot(p,n)) / glm::dot(dir,n);
    vec3 p0 = p + (dir * t);
    
    if (t < 1e-2) {
        if (t > 0) {
            //printf("triangle near zero = %f\n", t);
        }
        return false;
    }
    // check if p0 in triangle, because triangle is convex.
    // if p0 in triangle, then absolute area equals to area_vec length, otherwise not.
    
    vec3 area_vec(0,0,0);
    float abs_area = 0.0;
    for (int i = 0; i < 3; ++i) {
        abs_area += glm::length(glm::cross(p0-vertexes[i], p0-vertexes[(i+1)%3]));
        area_vec += glm::cross(p0-vertexes[i], p0-vertexes[(i+1)%3]);
    }
    
    vec3 tmp_nb = glm::cross(c-p0, a-p0);
    vec3 tmp_nc = glm::cross(a-p0, b-p0);
    
    float beta = glm::dot(n, tmp_nb) / glm::dot(n, n);
    float gamma = glm::dot(n, tmp_nc) / glm::dot(n, n);
    float alpha = 1.0 - beta - gamma;
    
    if (beta > -eps && beta < 1.0+eps && gamma > -eps && gamma < 1.0+eps 
        && alpha > -eps && alpha < 1.0+eps) {
        *dis_to_ray = t;
        return true;
    } else
        return false;
}
vec3 Triangle::InterpolatePointNormal(const vec3& point) const {
    vec3 p = vec3TimeMat4(point, this->InversedTransform);
    vec3 n = glm::cross(b-a, c-a);
    vec3 tmp_nb = glm::cross(c-p, a-p);
    vec3 tmp_nc = glm::cross(a-p, b-p);
    
    float beta = glm::dot(n, tmp_nb) / glm::dot(n,n);
    float gamma = glm::dot(n, tmp_nc) / glm::dot(n,n);
    float alpha = 1.0 - beta - gamma;
    vec3 ret = (na * alpha) + (nb * beta) + (nc * gamma);
    return vec3(vec4(ret, 0.0f) * glm::transpose(this->InversedTransform));
}

Object::~Object() {
}
Triangle::~Triangle() {
}
Sphere::~Sphere() {
}
