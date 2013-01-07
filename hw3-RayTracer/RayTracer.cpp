#include "RayTracer.h"
#include "stdio.h"

Ray RayTracer::GenerateRay(const Camera& camera, int i, int j, int width, int height) {
    vec3 w = glm::normalize(camera.eye - camera.center);
	vec3 u = glm::normalize(glm::cross(camera.up, w));
	vec3 v = glm::cross(w, u);
	
	float fovy = camera.fovy * PI / 180.0; // degree to radian
	float x_range = tan(fovy / 2.0) * width / height;
	float a =  x_range * (i - width/2.0) / (width / 2.0);
	float b = tan(fovy / 2.0) * (j - height/2.0) / (height / 2.0);
    return Ray(camera.eye, -w + u*a + v*b);
}

Color RayTracer::Trace(const Ray& ray, const Scene& scene, int depth) {
    
    if (depth > RAYTRACE_DEPTH_LIMIT) {
        return BLACK;
    }
    float nearest_dist = INF;
    const Object* hit_object(NULL);
    for (int i = 0; i < scene.objects.size(); ++i) {
        float dist;
        if (scene.objects[i]->Intersect(ray, &dist) && dist < nearest_dist) {
            nearest_dist = dist;
            hit_object = scene.objects[i];
        }
    }
    //printf("nearest_dist = %.3f\n", nearest_dist);
    if (hit_object == NULL)
        return BLACK;
    else
        return WHITE;
/*
    Color color(BLACK);
    Material material = hit_object->material();
    Vec3 normal = hit_object->normal();
    Point hit_point = ray.CalcPoint(nearest_dist);
    for (int i = 0; i < scene.lights.size(); ++i) {
        if (!scene.lights[i].IsBlocked(hit_point)) {
            color += scene.lights[i].CalcLight(material, normal, ray, hit_point);
        }
    }

    if (material.specular > 0) {
        Ray reflect_ray = ray.CreateReflectRay(hit_point, normal);
        // Make a recursive call to trace the reflected ray
        Color temp_color = Trace(reflect_ray, scene, depth+1);
        color += material.specular * temp_color;
    }

    return color;
*/
}



