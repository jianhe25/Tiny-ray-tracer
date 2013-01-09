#include "RayTracer.h"
#include "stdio.h"

Ray RayTracer::GenerateRay(const Camera& camera, int i, int j, int height, int width) {
    vec3 w = glm::normalize(camera.eye - camera.center);
	vec3 u = glm::normalize(glm::cross(camera.up, w));
	vec3 v = glm::cross(w, u);
	
	float fovy = camera.fovy * PI / 180.0; // degree to radian
	float x_range = tan(fovy / 2.0) * width / height;
	float b = tan(fovy / 2.0) * (i - height/2.0) / (height / 2.0);
	float a =  x_range * (j - width/2.0) / (width / 2.0);
    return Ray(camera.eye, -w + u*a + v*b);
}

bool GetIntersect(const Ray& ray, const Scene& scene, const Object* hit_object, double* nearest_dist) {
    *nearest_dist = INF;
    hit_object = NULL;
    for (int i = 0; i < scene.objects.size(); ++i) {
        float dist;
        if (scene.objects[i]->Intersect(ray, &dist) && dist < *nearest_dist) {
            *nearest_dist = dist;
            hit_object = scene.objects[i];
        }
    }
    if (hit_object == NULL)
        return false;
    else
        return true;
}

Color RayTracer::Trace(const Ray& ray, const Scene& scene, int depth) {
    
    if (depth > RAYTRACE_DEPTH_LIMIT) {
        return BLACK;
    }
    
    float nearest_dist;
    const Object* hit_object;
    if (!GetIntersect(ray, scene, hit_object, &nearest_dist))
        return BLACK;
    
    Color color(scene.ambient);
    Point hit_point = ray.o + ray.direction * nearest_dist;
    for (int i = 0; i < scene.lights.size(); ++i) {
        Ray light_ray(lights[i], hit_point-light[i]);
        const Object* tmp_obj;
        float tmp_dist;
        GetIntersect(light_ray, scene, tmp_obj, &tmp_dist);
        assert(tmp_obj != NULL);
        if (sgn(tmp_dist - nearest_dist) == 0) {
            color += CalcLight(scene.lights[i], hit_object, ray, hit_point);
        }
    }
    
/*
    if (materials.specular > 0) {
        Ray reflect_ray = ray.CreateReflectRay(hit_point, normal);
        // Make a recursive call to trace the reflected ray
        Color temp_color = Trace(reflect_ray, scene, depth+1);
        color += materials.specular * temp_color;
    }
*/
    return color;
}

Color RayTracer::CalcLight(const Light& light, const Object* hit_object, const Ray& ray, const vec3& hit_point) {
    
    vec3 light_direction;
    if (light.type == Light::point) {
        light_direction = glm::normalize(light.positionOrDirection - hit_point);
    } else 
        light_direction = glm::normalize(light.positionOrDirection);
    
    vec3 normal = hit_object->InterpolatePointNormal(hit_point);
    
    const Materials& materials = hit_object->materials;
	float nDotL = max(glm::dot(normal, light_direction), 0.0);
	vec3 diffuse = materials.diffuse * light.color * nDotL;
    
    vec3 halfvec = glm::normalize(light_direction + ray.direction);
	float nDotH = max(glm::dot(normal, halfvec), 0.0);
	vec3 specular = materials.specular * light.color * pow(nDotH, materials.shininess);
	
	return diffuse + specular;
}

