Image Raytrace (Camera cam, Scene scene, int width, int height) 
{ 
	Image image = new Image (width, height) ; 
	for (int i = 0 ; i < height ; i++)  
		for (int j = 0 ; j < width ; j++) { 
			Ray ray = cam.GenerateRay(i, j);
			image[i][j] = RayTracer.Trace(ray, scene);
		}
	return image;
}

#include "constants.h"
#include "basic_structs.h"

class RayTracer {
	Color Trace(const Ray& ray, const Scene& scene, int depth) {
		if (depth > RAYTRACE_DEPTH_LIMIT) {
			return BLACK;
		}
		double nearest_dist = INF;
		const Object* hit_object;
		for (int i = 0; i < scene.num_objects(); ++i) {
			double dist;
			if (scene.objects[i].Intersect(ray, &dist) && dist < nearest_dist) {
				nearest_dist = dist;
				hit_object = &scene.objects[i];
			}
		}
		if (hit_object == NULL)
			return Color::BLACK;

		Color color(Color::BLACK);
		Material material = hit_object->material();
		Vec3 normal = hit_object->normal();
		Point hit_point = ray.CalcPoint(nearest_dist);
		for (int i = 0; i < scene.num_lights(); ++i) {
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
	}
};


