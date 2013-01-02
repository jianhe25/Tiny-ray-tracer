// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Transform.h" 

using namespace std ;
#include "variables.h" 
#include "readfile.h"



void rightmultiply(const mat4 & M, stack<mat4> &transform_stack) {
	mat4 &T = transform_stack.top() ; 
	// Right multiply M, but do this left to account for row/column major 
	T = M * T ; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool Scene::readvals(stringstream &s, const int numvals, GLfloat *values) {
	for (int i = 0 ; i < numvals ; i++) {
		s >> values[i] ; 
		if (s.fail()) {
			cout << "Failed reading value " << i << " will skip\n" ; 
			return false ;
		}
	}
	return true ; 
}

void Scene::readfile(const char *filename) {
	string str, cmd ; 
	ifstream in ;
	in.open(filename); 
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
		if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
		// Ruled out comment and blank lines 
		stringstream s(str) ;
		s >> cmd; 
		int i; 
		GLfloat values[10] ; // position and color for light, colors for others
		// Up to 10 params for cameras.  
		bool validinput ; // validity of input 

		// Process the light, add it to database.
		// Lighting Command
		if (cmd == "light") {
			if (numused == numLights) // No more Lights 
				cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n" ;
			else {
				validinput = readvals(s, 8, values) ; // Position/color for lts.
				if (validinput) {
					for (int i = 0; i < 4; ++i) {
						lightposn[i] = values[i];
						lightcolor[i] = values[i+4];
					}
					++numused ; 
				}
			}
		}
		// Material Commands 
		// Ambient, diffuse, specular, shininess
		// Filling this in is pretty straightforward, so I've left it in 
		// the skeleton, also as a hint of how to do the more complex ones.
		// Note that no transforms/stacks are applied to the colors. 
		else if (cmd == "ambient") {
			validinput = readvals(s, 4, values) ; // colors 
			if (validinput) 
			for (i = 0 ; i < 4 ; i++) ambient[i] = values[i] ; 
		} 
		else if (cmd == "diffuse") {
			validinput = readvals(s, 4, values) ; 
			if (validinput) 
			for (i = 0 ; i < 4 ; i++) diffuse[i] = values[i] ; 
		}
		else if (cmd == "specular") {
			validinput = readvals(s, 4, values) ; 
			if (validinput) 
			for (i = 0 ; i < 4 ; i++) specular[i] = values[i] ; 
		}
		else if (cmd == "emission") {
			validinput = readvals(s, 4, values) ; 
			if (validinput) 
			for (i = 0 ; i < 4 ; i++) emission[i] = values[i] ; 
		}
		else if (cmd == "shininess") {
			validinput = readvals(s, 1, values) ; 
			if (validinput) shininess = values[0] ; 
		}
		else if (cmd == "size") {
			validinput = readvals(s,2,values) ; 
			if (validinput) { 
				width = (int) values[0] ; 
				height = (int) values[1] ; 
			}
		}
		else if (cmd == "camera") {
			validinput = readvals(s,10,values) ; // 10 values eye cen up fov
			if (validinput) {
				eyeinit = vec3(values[0], values[1], values[2]);
				upinit = vec3(values[3], values[4], values[5]);
				center = vec3(values[6], values[7], values[8]);
				fovy = values[9];
			}
		}
		else if (cmd == "vertex") {

		}
		// I've left the code for loading objects in the skeleton, so 
		// you can get a sense of how this works.  
		else if (cmd == "sphere" || cmd == "cube" || cmd == "teapot") {
			if (numobjects == maxobjects) // No more objects 
				cerr << "Reached Maximum Number of Objects " << numobjects << " Will ignore further objects\n" ; 
			else {
				validinput = readvals(s, 1, values) ; 
				if (validinput) {
					object *obj = &(objects[numobjects]) ; 
					obj -> size = values[0] ; 
					for (i = 0 ; i < 4 ; i++) {
						(obj -> ambient)[i] = ambient[i] ; 
						(obj -> diffuse)[i] = diffuse[i] ; 
						(obj -> specular)[i] = specular[i] ; 
						(obj -> emission)[i] = emission[i] ;
					}
					obj -> shininess = shininess ; 
					obj -> transform = transform_stack.top() ; 
					if (cmd == "sphere") obj -> type = sphere ; 
					else if (cmd == "cube") obj -> type = cube ; 
					else if (cmd == "teapot") obj -> type = teapot ; 
				}
				++numobjects ; 
			}
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
	// Set up initial position for eye, up and amount
	// As well as booleans 
	eye = eyeinit; 
	up = upinit; 
}
