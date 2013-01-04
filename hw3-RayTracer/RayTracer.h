#include "scene.h"
#ifndef _RAY_TRACER_H_
#define _RAY_TRACER_H_

struct Ray {
    vec3 origin;
    vec3 direction;
    Ray(const vec3& _origin, const vec3& _direction);
};

class RayTracer {
public:
    Color Trace(const Ray& ray, const Scene& scene, int depth);
    Ray GenerateRay(const Camera& camera, int i, int j, int width, int height);
};
#endif // _RAY_TRACER_H_
