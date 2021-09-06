#include <iostream>

import hb_math;
import graphics;
import ModelImporter;
import volumes;

using namespace gl;
using namespace hb_math;
using namespace volumes;


#define sphere_count 5
int main()
{
	
	glInit();
	gl::spheres = new sphere[sphere_count];
	gl::vol_size = sphere_count;
	vect3 center;
	material white;
	material black;
	white.diffuse.x = white.diffuse.y = white.diffuse.z = 1;
	black.diffuse.x = black.diffuse.y = black.diffuse.z = 0.5;
	center.z = -40;
	center.y = -5;
	gl::spheres[0].center = center;
	gl::spheres[0].radius = 6;
	gl::spheres[0].mat = white;
	center.y = -1;
	gl::spheres[1].center = center;
	gl::spheres[1].radius = 5;
	gl::spheres[1].mat = white;
	center.y = 1.5;
	gl::spheres[2].center = center;
	gl::spheres[2].radius = 4;
	gl::spheres[2].mat = white;
	center.y = 1.5;
	center.z = -20;
	center.x = -0.2;
	gl::spheres[3].center = center;
	gl::spheres[3].radius = 1;
	gl::spheres[3].mat = black;
	center.x = 0.2;
	gl::spheres[4].center = center;
	gl::spheres[4].radius = 1;
	gl::spheres[4].mat = black;
	render();
	glFinish("output.bmp", false);
	return 0;
}