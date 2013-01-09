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

bool RayTracer::GetIntersection(const Ray& ray, const Scene& scene, const Object* &hit_object, float* nearest_dist) {
    *nearest_dist = INF;
    hit_object = NULL;
    for (int i = 0; i < (int)scene.objects.size(); ++i) {
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
int debugCount = 0;

void debug(Color color) {
    printf("(%.2f %.2f %.2f)\n",color.r, color.g, color.b);
}
void debug(vec3 vec) {
    printf("(%.2f %.2f %.2f)\n",vec.x, vec.y, vec.z);
}
Color RayTracer::Trace(const Ray& ray, const Scene& scene, int depth) {
    
    if (depth > RAYTRACE_DEPTH_LIMIT) {
        return BLACK;
    }
    
    float nearest_dist;
    const Object* hit_object;
    if (!GetIntersection(ray, scene, hit_object, &nearest_dist))
        return BLACK;
    
    if (ray.direction == vec3(0,0,-1)) {
        printf("nearest_dist = %f\n",nearest_dist);
        if (hit_object->type == Object::sphere)
            printf("sphere\n");
        else
            printf("triangle\n");
    }
    
    Color color(hit_object->materials.ambient);
    vec3 hit_point = ray.o + ray.direction * nearest_dist;
    
    //printf("light number = %d\n", (int)scene.lights.size());
    for (int i = 0; i < (int)scene.lights.size(); ++i) {
        if (scene.lights[i].type == Light::point) {
            Ray light_ray(scene.lights[i].position(),
                          hit_point - scene.lights[i].position());
            const Object* tmp_obj;
            float tmp_dist;
            GetIntersection(light_ray, scene, tmp_obj, &tmp_dist);
            assert(tmp_obj != NULL);
            vec3 light_hit = light_ray.o + light_ray.direction * tmp_dist;
            /*
            if (++debugCount < 10) {
                debug(light_hit);
                debug(hit_point);
                puts("");
            }
            */
             //   printf("tmp_dist = %.2f %.2f\n",tmp_dist, nearest_dist);
            if (IsSameVector(hit_point, light_hit)) {
                //puts("point light source");
                color = color + CalcLight(scene.lights[i], hit_object, ray, hit_point);
            }
        } else {
            color = color + CalcLight(scene.lights[i], hit_object, ray, hit_point);
        }
    }
    /*
    if (++debugCount < 10) {
        debug(color);
	}
	*/
    return color;
/*
    if (materials.specular > 0) {
        Ray reflect_ray = ray.CreateReflectRay(hit_point, normal);
        // Make a recursive call to trace the reflected ray
        Color temp_color = Trace(reflect_ray, scene, depth+1);
        color += materials.specular * temp_color;
    }
*/
    
}


Color RayTracer::CalcLight(const Light& light, const Object* hit_object, const Ray& ray, const vec3& hit_point) {
    
    vec3 light_direction;
    if (light.type == Light::point) {
        light_direction = glm::normalize(light.positionOrDirection - hit_point);
    } else 
        light_direction = glm::normalize(light.positionOrDirection);
    
    vec3 normal = glm::normalize(hit_object->InterpolatePointNormal(hit_point));
    
    const Materials& materials = hit_object->materials;
	float nDotL = max(glm::dot(normal, light_direction), 0.0f);
	Color diffuse = materials.diffuse * light.color * nDotL;
    
    vec3 halfvec = glm::normalize(light_direction + ray.direction);
	float nDotH = max(glm::dot(normal, halfvec), 0.0f);
	Color specular = materials.specular * light.color * pow(nDotH, materials.shininess);
	
	
	return diffuse + specular;
}

