#include "RayTracer.h"
#include <stdio.h>
class RayTracerTester {
public:
    RayTracer* ray_tracer;
    RayTracerTester() {
        ray_tracer = new RayTracer();
    }
    ~RayTracerTester() {
        delete ray_tracer;
    }
    void GenerateRayTest() {
        Camera camera(vec3(0,0,5), vec3(0,0,0), vec3(0,1,0), 30);
        Ray ray = ray_tracer->GenerateRay(camera, 640/2, 480/2, 640, 480);
        assert(ray.o == vec3(0,0,5));
        assert(ray.direction == vec3(0,0,-1));
        
        Camera camera2(vec3(0,0,5), vec3(0,0,0), vec3(0,1,0), 90);
        ray = ray_tracer->GenerateRay(camera2, 0, 0, 640, 480);
        assert(glm::length(ray.direction - vec3(-480.0 / 640.0, 1, -1)) < 1e-3);
        
        //assert(ray.direction == glm::normalize(vec3(-640.0 / 480.0, -1, -1)));
    }
};
