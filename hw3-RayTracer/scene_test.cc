#include "scene.h"
#include "object.h"
#include <iostream>
namespace SceneTester {
    static void ReadFileTest() {
        string file_name = "scene_read_test.txt";
        
        Scene scene;
        scene.readfile(file_name);
    
        assert(scene.camera.eye == vec3(-4,-4,4));
        assert(scene.camera.center == vec3(1,0,0));
        assert(scene.camera.up == vec3(0,1,0));
        assert(scene.camera.fovy == 30.0);
        
        assert(scene.lights.size() == 2);
        
        assert(scene.lights[0].type == Light::directional);
        assert(scene.lights[0].positionOrDirection == vec3(0, 0, 1));
        assert(scene.lights[0].color == Color(0.5, 0.5, 0.5)); 
        
        assert(scene.lights[1].type == Light::point);
        assert(scene.lights[1].positionOrDirection == vec3(4,0,4));
        assert(scene.lights[1].color == Color(0.5, 0.5, 0.5));
        
        assert(scene.materials.diffuse == Color(1, 0, 0));
        assert(scene.materials.specular == Color(0, 0, 0));
        assert(scene.materials.ambient == Color(0.1, 0.1, 0.1));
        

        assert(scene.vertexBuffer.size() == 4);
        assert(scene.objects.size() == 2);
        assert(scene.objects[0]->type == Object::triangle);
        
        Triangle* triangle = dynamic_cast<Triangle*>(scene.objects[0]);
        if (triangle == 0) {
            cout << "Null pointer on first type-cast" << endl;
            return;
        }
        assert(triangle->vertexes[0] == scene.vertexBuffer[0]);
        assert(triangle->vertexes[1] == scene.vertexBuffer[1]);
        assert(triangle->vertexes[2] == scene.vertexBuffer[2]);
        
        assert(scene.objects[1]->type == Object::triangle);
        triangle = dynamic_cast<Triangle*>(scene.objects[1]);
        if (triangle == NULL) {
            cout << "Nul1 pointer on second type-cast" << endl;
            return;
        }
        assert(triangle->vertexes[0] == scene.vertexBuffer[0]);
        assert(triangle->vertexes[1] == scene.vertexBuffer[2]);
        assert(triangle->vertexes[2] == scene.vertexBuffer[3]);
    }
}

