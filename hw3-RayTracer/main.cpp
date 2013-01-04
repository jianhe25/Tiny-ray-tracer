#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glew.h>
#include <GL/glut.h>
#include "shaders.h"
#include "Transform.h"
#include <FreeImage.h>
#include <stdio.h>

using namespace std;


void SaveScreenshot(string fname, const BYTE* image) {
	
	FIBITMAP* img = FreeImage_ConvertFromRawBits(image, w, h, w * 3, 
		24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	
	std::cout << "Saving screenshot: " << fname << "\n";

	FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
	delete pixels;
}

BYTE* RayTrace (Camera camera, Scene scene, int width, int height)  {
	int pix = width * height;
	BYTE* image = new BYTE[3*pix];
	for (int i = 0 ; i < height ; i++)  
		for (int j = 0 ; j < width ; j++) { 
			Ray ray = camera.GenerateRay(i, j);
			Color color = RayTracer.Trace(ray, scene);
			int base = 3 * (i * width + j);
			image[base + 0] = color.Rbyte();
			image[base + 1] = color.Gbyte();
			image[base + 2] = color.Bbyte();
		}
	return image;
}
/*
int main(int argc, char* argv[]) {

	if (argc < 2) {
		cerr << "Usage: transforms scenefile [grader input (optional)]\n"; 
		exit(-1); 
	}

  	FreeImage_Initialise();
	
	Scene scene;
    scene.readfile(argv[1]);
    BYTE* image = RayTrace(scene.camera, scene, width, height);
    SaveScreenshot("result.png", image);

	FreeImage_DeInitialise();
	return 0;
}
*/
