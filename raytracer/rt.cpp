#include <iostream>

import hb_math;
import graphics;
import ModelImporter;
import volumes;

using namespace gl;
using namespace hb_math;
using namespace volumes;


#define sphere_count 15
int main()
{
	
	glInit(1920, 1080);
	gl::spheres = new sphere[sphere_count];
	gl::vol_size = sphere_count;
	vect3 center;
	material white;
	material black;
	material carrot;
	material eyes;
	white.diffuse.x = white.diffuse.y = white.diffuse.z = 1;
	black.diffuse.x = black.diffuse.y = black.diffuse.z = 0;
	eyes.diffuse.x = eyes.diffuse.y = eyes.diffuse.z = 0.8;
	carrot.diffuse.x = 1;
	carrot.diffuse.y = 0.5;
	center.z = -60;
	center.y = 25;
	gl::spheres[0].center = center;
	gl::spheres[0].radius = 6;
	gl::spheres[0].mat = white;
	center.y = 10;
	gl::spheres[1].center = center;
	gl::spheres[1].radius = 10;
	gl::spheres[1].mat = white;
	center.y = -10;
	gl::spheres[2].center = center;
	gl::spheres[2].radius = 15;
	gl::spheres[2].mat = white;

	center.z = -20;
	center.y = 8.5;
	gl::spheres[3].center = center;
	gl::spheres[3].radius = 0.2;
	gl::spheres[3].mat = carrot;
	center.y = 4;
	gl::spheres[4].center = center;
	gl::spheres[4].radius = 0.4;
	gl::spheres[4].mat = black;
	center.y = 1;
	gl::spheres[5].center = center;
	gl::spheres[5].radius = 0.4;
	gl::spheres[5].mat = black;
	center.y = -4;
	gl::spheres[6].center = center;
	gl::spheres[6].radius = 0.4;
	gl::spheres[6].mat = black;
	center.y = 8.7;
	center.x = 1;
	gl::spheres[7].center = center;
	gl::spheres[7].radius = 0.2;
	gl::spheres[7].mat = eyes;
	center.x *= -1;
	gl::spheres[8].center = center;
	gl::spheres[8].radius = 0.2;
	gl::spheres[8].mat = eyes;
	center.y = 7;
	center.x = 0.3;
	gl::spheres[9].center = center;
	gl::spheres[9].radius = 0.2;
	gl::spheres[9].mat = black;
	center.x *= -1;
	gl::spheres[10].center = center;
	gl::spheres[10].radius = 0.2;
	gl::spheres[10].mat = black;
	center.y = 7.4;
	center.x = 0.7;
	gl::spheres[11].center = center;
	gl::spheres[11].radius = 0.2;
	gl::spheres[11].mat = black;
	center.x *= -1;
	gl::spheres[12].center = center;
	gl::spheres[12].radius = 0.2;
	gl::spheres[12].mat = black;
	center.z = -18;
	center.y = 7.9;
	center.x = 0.9;
	gl::spheres[13].center = center;
	gl::spheres[13].radius = 0.1;
	gl::spheres[13].mat = black;
	center.x *= -1;
	gl::spheres[14].center = center;
	gl::spheres[14].radius = 0.1;
	gl::spheres[14].mat = black;
	render();
	glFinish("output.bmp", false);
	return 0;
}