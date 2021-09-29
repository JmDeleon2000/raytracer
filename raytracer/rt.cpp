#include <iostream>

import hb_math;
import graphics;
import ModelImporter;
import volumes;

using namespace gl;
using namespace hb_math;
using namespace volumes;


#define sphere_count 3
int main()
{
	
	glInit(1920, 1080);
	gl::enviromentMap = new modelImp::texture("room.bmp");
	gl::spheres = new sphere[sphere_count];
	gl::vol_size = sphere_count;
	vect3 center;
	vect3 dirLightColor;
	vect3 dirLightDirection;
	dirLightDirection.z = 0.2;
	dirLightDirection.z = -1;
	dirLightColor.y = 1;
	material white;
	white.diffuse.x = white.diffuse.y = white.diffuse.z = 1;
	white.specular = 256;
	white.type = OPAQUE;


	gl::setAmbientLight(white.diffuse, 0.1);
	gl::setDirLight(dirLightColor, dirLightDirection, 0.5);
	vect3 pointLightPosition;
	vect3 pointLightColor;
	pointLightPosition.x = -30;
	pointLightPosition.z = 20;
	pointLightPosition.y = 0;
	pointLightColor.z = 1;
	gl::setPointLights(pointLightColor, pointLightPosition, 0.5);
	center.z = -60;
	center.x = 0;
	gl::spheres[0].center = center;
	gl::spheres[0].radius = 10;
	gl::spheres[0].mat = white;

	center.z = -30;
	center.x = -3;
	gl::spheres[1].center = center;
	gl::spheres[1].radius = 1;
	gl::spheres[1].mat = white;


	center.z = -30;
	center.x = 10;
	white.type = REFLECTIVE;
	gl::spheres[2].center = center;
	gl::spheres[2].radius = 7;
	gl::spheres[2].mat = white;

	render();
	glFinish("output.bmp", false);
	return 0;
}