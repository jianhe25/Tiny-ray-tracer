#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "Transform.h"
#include <FreeImage.h>
#include <stdio.h>

using namespace std;

#include "scene_test.cc"
#include "RayTracer.h"
#include "RayTracer_test.cc"

void SaveScreenshot(string fname, BYTE* image, int width, int height) {
	
	FIBITMAP* img = FreeImage_ConvertFromRawBits(image, width, height, 
        width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	
	std::cout << "Saving screenshot: " << fname << "\n";

	FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
}

BYTE* RayTrace (Camera camera, const Scene& scene)  {
	RayTracer ray_tracer;
	int width = scene.width;
	int height = scene.height;
	int pix = width * height;
	BYTE* image = new BYTE[3*pix];
	for (int i = 0 ; i < height ; i++)  
		for (int j = 0 ; j < width ; j++) { 
			Ray ray = ray_tracer.GenerateRay(camera, i, j, width, height);
			Color color = ray_tracer.Trace(ray, scene, 0);
			int base = 3 * (i * width + j);
			image[base + 0] = color.Rbyte();
			image[base + 1] = color.Gbyte();
			image[base + 2] = color.Bbyte();
		}
	return image;
}

void RunTest() {
    SceneTester::ReadFileTest();
    RayTracerTester ray_tracer_tester;
    ray_tracer_tester.GenerateRayTest();
}

int main(int argc, char* argv[]) {
    RunTest();
	if (argc < 2) {
		cerr << "Usage: transforms scenefile [grader input (optional)]\n"; 
		exit(-1); 
	}
  	FreeImage_Initialise();
	
	Scene scene;
    scene.readfile(argv[1]);
    
    BYTE* image = RayTrace(scene.camera, scene);
    SaveScreenshot("result.png", image, scene.width, scene.height);
    delete image;
    
	FreeImage_DeInitialise();
	return 0;
}

