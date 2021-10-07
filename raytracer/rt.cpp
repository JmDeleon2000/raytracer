#include <iostream>

import hb_math;
import graphics;
import ModelImporter;
import volumes;

using namespace gl;
using namespace hb_math;
using namespace volumes;


#define sphere_count 2
int main()
{
	
	glInit(1920, 1080);
	gl::enviromentMap = new modelImp::texture("room.bmp");
	gl::vols = new volume*[sphere_count];
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
	mate.Texture = gl::enviromentMap;

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
	mirror.ior = 200;

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
	//gl::vols[0] = new sphere(10, center, water);


	center.x = 15;
	//gl::vols[1] = new sphere(10, center, emerald);

	center.x = 45;
	gl::vols[0] = new sphere(10, center, mirror);

	//gl::STEPS = 3;
	vect3 A, B, C, normal;
	//normal.x = 1;
	normal.y = -100;
	normal.z = -1;
	//center.y = 0;
	//center.z = -445;
	A = center;
	A.x = 60;

	B.x = 0;
	B.y = 10;
	B.z = -100;

	C.x = 100;
	C.y = 100;
	C.z = -200;
	
	//gl::vols[1] = new triangle(A, C, B,  mate, false);
	//gl::vols[1] = new plane(center, normal, mate);

	center.x = 75;
	//gl::vols[3] = new sphere(10, center, mate);

	center.x = -45;
	//gl::vols[4] = new sphere(10, center, aluminum);


	center.x = -75;
	//gl::vols[5] = new sphere(10, center, skin);


	render();
	glFinish("output.bmp", false);
	return 0;
}