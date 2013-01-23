// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "Transform.h" 

using namespace std;
#include "scene.h" 

const vec3& Light::position() const {
    return positionOrDirection;
}
const vec3& Light::direction() const {
    return positionOrDirection;
}

void rightmultiply(const mat4 & M, stack<mat4> &transform_stack) {
	mat4 &T = transform_stack.top() ; 
	// Right multiply M, but do this left to account for row/column major 
	T = M * T ; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool Scene::readvals(stringstream &s, const int numvals, float *values) {
	for (int i = 0 ; i < numvals ; i++) {
		s >> values[i] ; 
		if (s.fail()) {
			cout << "Failed reading value " << i << " will skip\n" ; 
			return false ;
		}
	}
	return true ; 
}

void Scene::readfile(const string &filename) {
	string str, cmd ; 
	ifstream in ;
	in.open(filename.c_str()); 
	if (!in.is_open()) {
		cerr << "Open " << filename << " failed!" << endl;
		throw 2;
	}
	// I need to implement a matrix stack to store transforms.  
	// This is done using standard STL Templates 
	stack<mat4> transform_stack ; 
	transform_stack.push(mat4(1.0)) ;  // identity

	getline (in, str) ; 
	while (in) {
		if (!((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#'))) {
            getline(in, str);
            continue;
		}
		// Ruled out comment and blank lines 
		stringstream s(str);
		s >> cmd; 
		int i; 
		float values[10]; // position and color for light, colors for others
		// Up to 10 params for cameras.  
		bool validinput ; // validity of input 
        
        if (cmd == "output") {
            s >> outputFile;
        }
		// Process the light, add it to database.
		// directional light
		else if (cmd == "directional" || cmd == "point") {
            validinput = readvals(s, 6, values) ; // Position/color for lts.
            if (validinput) {
                Light light;
                light.positionOrDirection = vec3(values[0], values[1], values[2]);
                light.color = Color(values[3], values[4], values[5]);
                if (cmd == "directional")
                    light.type = Light::directional;
                else if (cmd == "point")
                    light.type = Light::point;                
                lights.push_back(light);
            }
		}
		else if (cmd == "attenuation") {
		    validinput = readvals(s, 3, values);
		    if (validinput) 
                for (i = 0; i < 3; ++i) attenuation[i] = values[i];
		}
		// materials Commands 
		// Ambient, diffuse, specular, shininess
		// Filling this in is pretty straightforward, so I've left it in 
		// the skeleton, also as a hint of how to do the more complex ones.
		// Note that no transformrs/stacks are applied to the colors. 
		else if (cmd == "ambient") {
			validinput = readvals(s, 3, values) ; // colors 
			if (validinput) 
                materials.ambient = Color(values[0], values[1], values[2]); 
		}
		else if (cmd == "diffuse") {
			validinput = readvals(s, 3, values) ; 
			if (validinput) 
                materials.diffuse = Color(values[0], values[1], values[2]); 
		}
		else if (cmd == "specular") {
			validinput = readvals(s, 3, values) ; 
			if (validinput) 
                materials.specular = Color(values[0], values[1], values[2]); 
		}
		else if (cmd == "emission") {
			validinput = readvals(s, 3, values) ; 
			if (validinput) 
                materials.emission = Color(values[0], values[1], values[2]); 
		}
		else if (cmd == "shininess") {
			validinput = readvals(s, 1, values) ; 
			if (validinput) 
                materials.shininess = values[0] ; 
		}
		else if (cmd == "size") {
			validinput = readvals(s, 2, values) ; 
			if (validinput) { 
				width = (int) values[0] ; 
				height = (int) values[1] ; 
			}
		}
		else if (cmd == "camera") {
			validinput = readvals(s, 10, values) ; // 10 values eye cen up fov
			if (validinput) {
				camera.eye = vec3(values[0], values[1], values[2]);
				camera.center = vec3(values[3], values[4], values[5]);
				camera.up = vec3(values[6], values[7], values[8]);
				camera.fovy = values[9];
			}
		}
		else if (cmd == "maxdepth") {
			validinput = readvals(s, 1, values);
			if (validinput) maxDepth = values[0];
		}
		else if (cmd == "vertex") {
			validinput = readvals(s, 3, values);
			if (validinput) vertexBuffer.push_back(vec3(values[0], values[1], values[2]));
		}
		else if (cmd == "vertexnormal") {
			validinput = readvals(s, 6, values);
			if (validinput) {
				vertexBufferWithNormal.push_back(vec3(values[0], values[1], values[2]));
				vertexNormalBuffer.push_back(vec3(values[3], values[4], values[5]));
			}
		}
		else if (cmd == "tri") {
			validinput = readvals(s, 3, values);
			if (validinput) {
				Triangle* triangle = new Triangle(
                    vertexBuffer[values[0]], vertexBuffer[values[1]], vertexBuffer[values[2]]);
				//printf("triangle.shineness = %f\n",triangle->materials.shininess);
				objects.push_back(triangle);
				objects.back()->index = objects.size();
				objects.back()->materials = materials;
				objects.back()->transform = transform_stack.top();
				objects.back()->InversedTransform = glm::inverse(transform_stack.top());
			}
		}
		else if (cmd == "trinormal") {
			validinput = readvals(s, 6, values);
			if (validinput) {
				Triangle* triangle = new Triangle(vertexBufferWithNormal[values[0]], 
						vertexBufferWithNormal[values[1]], 
						vertexBufferWithNormal[values[2]],
						vertexNormalBuffer[values[0]],
						vertexNormalBuffer[values[1]],
						vertexNormalBuffer[values[2]]
						);
				objects.push_back(triangle);
				objects.back()->index = objects.size();
				objects.back()->materials = materials;
				objects.back()->transform = transform_stack.top();
				objects.back()->InversedTransform = glm::inverse(transform_stack.top());
			}
		}

		else if (cmd == "sphere") {
			validinput = readvals(s, 4, values);
			Sphere* sphere = new Sphere(vec3(values[0], values[1], values[2]), values[3]);
			objects.push_back(sphere);
			objects.back()->index = objects.size();
            objects.back()->materials = materials;
            objects.back()->transform = transform_stack.top();
            objects.back()->InversedTransform = glm::inverse(transform_stack.top());
		}
		
		else if (cmd == "maxverts" || cmd == "maxvertnorms") { // just ignore
		}

		else if (cmd == "translate") {
			validinput = readvals(s,3,values) ; 
			if (validinput) {
				rightmultiply(Transform::translate(values[0], values[1], values[2]), transform_stack);
			}
		}
		else if (cmd == "scale") {
			validinput = readvals(s,3,values) ; 
			if (validinput) {
				rightmultiply(Transform::scale(values[0], values[1], values[2]), transform_stack);
			}
		}
		else if (cmd == "rotate") {
			validinput = readvals(s,4,values) ; 
			if (validinput) {
				mat3 rot3 = Transform::rotate(values[3], vec3(values[0], values[1], values[2]));
				mat4 rot4(rot3);
				rightmultiply(rot4, transform_stack);
			}
		}

		// I include the basic push/pop code for matrix stacks
		else if (cmd == "pushTransform") 
			transform_stack.push(transform_stack.top()) ; 
		else if (cmd == "popTransform") {
			if (transform_stack.size() <= 1) 
				cerr << "Stack has no elements.  Cannot Pop\n" ; 
			else transform_stack.pop() ; 
		}

		else {
			cerr << "Unknown Command: " << cmd << " Skipping \n" ; 
		}
		getline (in, str) ; 
    }
}
Scene::Scene() {
    attenuation[2] = 0.0;
    attenuation[1] = 0.0;
    attenuation[0] = 1.0;
    maxDepth = 5;
}

Scene::~Scene() {
}
