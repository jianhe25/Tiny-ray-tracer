#ifndef _OBJECT_TEST_CC
#define _OBJECT_TEST_CC

#include "object.h"
#include <iostream>
namespace ObjectTester {
    
    static void TriangleIntersectionTest() {
        Triangle triangle(vec3(-1,-1,0), vec3(1,-1,0), vec3(0,1,0));
        Ray ray(vec3(0,0,3), vec3(0,0,-1));
        Object* object = &triangle;
        float dist = INF;
        assert(object->Intersect(ray, &dist) && sgn(dist - 3.0) == 0);        
    }
    
    static void SphereIntersectionTest() {
        Sphere sphere(vec3(0,0,0), 1.0);
        Ray ray(vec3(5,0,5), glm::normalize(vec3(-1,0,-1)));
        Object* object = &sphere;
        float dist = INF;
        assert(object->Intersect(ray, &dist) && sgn(dist - (5*sqrt(2.0)-1.0)) == 0);
    }
};
#endif // _OBJECT_TEST_CC
