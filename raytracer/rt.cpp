#include <iostream>

import hb_math;
import graphics;
import ModelImporter;
import volumes;

using namespace gl;
using namespace hb_math;
using namespace volumes;


#define sphere_count 6
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
	dirLightColor.x = dirLightColor.y = dirLightColor.z = 1;
	material water;
	water.diffuse.x = water.diffuse.y = water.diffuse.z = 1;
	water.specular = 64;
	water.type = TRANSPARENT;
	water.ior = 1.333f;

	material emerald;
	emerald.diffuse.y = 1;
	emerald.specular = 254;
	emerald.type = TRANSPARENT;
	emerald.ior = 1.605f;

	material mate;
	mate.diffuse.x = mate.diffuse.y = mate.diffuse.z = 0.5f;
	mate.specular = 1;
	mate.type = OPAQUE;

	material skin;
	skin.diffuse.x = 1;
	skin.diffuse.y = 0.85f;
	skin.diffuse.z = 0.67f;
	skin.specular = 32;
	skin.type = OPAQUE;

	material mirror;
	mirror.diffuse.x = mirror.diffuse.y = mirror.diffuse.z = 1;
	mirror.specular = 128;
	mirror.type = REFLECTIVE;

	material aluminum;
	aluminum.diffuse.x = 0.913f;
	aluminum.diffuse.y = 0.921f;
	aluminum.diffuse.z = 0.925f;
	aluminum.specular = 64;
	aluminum.type = REFLECTIVE;

	gl::setAmbientLight(water.diffuse, 0.1);
	gl::setDirLight(dirLightColor, dirLightDirection, 0.5);
	vect3 pointLightPosition;
	vect3 pointLightColor;
	pointLightPosition.x = -30;
	pointLightPosition.z = 20;
	pointLightPosition.y = 0;
	pointLightColor.x = pointLightColor.y = pointLightColor.z = 1;
	gl::setPointLights(pointLightColor, pointLightPosition, 0.5);





	center.z = -100;
	center.x = -15;
	gl::spheres[0].center = center;
	gl::spheres[0].radius = 10;
	gl::spheres[0].mat = water;

	center.x = 15;
	gl::spheres[1].center = center;
	gl::spheres[1].radius = 10;
	gl::spheres[1].mat = emerald;

	center.x = 45;
	gl::spheres[2].center = center;
	gl::spheres[2].radius = 10;
	gl::spheres[2].mat = mirror;

	center.x = 75;
	gl::spheres[3].center = center;
	gl::spheres[3].radius = 10;
	gl::spheres[3].mat = mate;

	center.x = -45;
	gl::spheres[4].center = center;
	gl::spheres[4].radius = 10;
	gl::spheres[4].mat = aluminum;

	center.x = -75;
	gl::spheres[5].center = center;
	gl::spheres[5].radius = 10;
	gl::spheres[5].mat = skin;

	render();
	glFinish("output.bmp", false);
	return 0;
}