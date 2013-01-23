#include "scene.h"
#ifndef _RAY_TRACER_H_
#define _RAY_TRACER_H_


class RayTracer {
public:
    Color Trace(const Ray& ray, const Scene& scene, int depth, int i, int j);
    
    Ray GenerateRay(const Camera& camera, int i, int j, int height, int width);
    
    bool GetIntersection(const Ray& ray, const Scene& scene, 
                    const Object* &hit_object, vec3* hit_point);
                 
    Color CalcLight(const Light& light, const Object* hit_object, 
                    const Ray& ray, const vec3& hit_point, const float* attenuation);
    
    Ray TransformRay(const Ray& ray, const Object* object);
    
    Ray CreateReflectRay(const Ray& ray, const vec3& hit, const vec3& unit_normal);
};
#endif // _RAY_TRACER_H_
