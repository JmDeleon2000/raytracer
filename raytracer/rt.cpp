#include <iostream>

import hb_math;
import graphics;
import ModelImporter;
import volumes;

using namespace gl;
using namespace hb_math;
using namespace volumes;


#define sphere_count 23
//stuff 8
//apples 15
int main()
{
	
	glInit(1920, 1080);
	gl::enviromentMap = new modelImp::texture("envMapProyecto.bmp");
	gl::vols = new volume*[sphere_count];
	gl::vol_size = sphere_count;
	
	vect3 dirLightColor;
	vect3 dirLightDirection;
	dirLightDirection.y = -10;
	dirLightDirection.z = -1;
	dirLightColor.x = dirLightColor.y = 1;
	dirLightColor.z = 0.8;
	material water;
	water.diffuse.x = water.diffuse.y = water.diffuse.z = 1;
	water.specular = 64;
	water.type = TRANSPARENT;
	water.ior = 1.333f;

	material greenapple;
	greenapple.diffuse.y = 1;
	greenapple.specular = 32;
	greenapple.type = OPAQUE;
	
	material redapple;
	redapple.diffuse.x = 1;
	redapple.specular = 32;
	redapple.type = OPAQUE;

	material plastic;
	plastic.diffuse.x = plastic.diffuse.y = plastic.diffuse.z = 0.7f;
	plastic.specular = 64;
	plastic.type = TRANSPARENT;
	plastic.Texture = new modelImp::texture("transPaperTexture.bmp");
	


	material trapo;
	trapo.specular = 2;
	trapo.type = OPAQUE;
	trapo.Texture = new modelImp::texture("trapo.bmp");
	
	material paper;
	paper.diffuse.x = paper.diffuse.y = paper.diffuse.z = 0.7f;
	paper.specular = 64;
	paper.type = OPAQUE;

	material counter;
	counter.specular = 64;
	counter.type = OPAQUE;
	counter.Texture = gl::enviromentMap;
	counter.diffuse.x = counter.diffuse.y = counter.diffuse.z = 1;

	material darkwood;
	darkwood.specular = 124;
	darkwood.diffuse.x = darkwood.diffuse.y = darkwood.diffuse.z = 0.2;


	gl::setAmbientLight(water.diffuse, 0.1);
	gl::setDirLight(dirLightColor, dirLightDirection, 0.7);
	vect3 pointLightPosition;
	vect3 pointLightColor;
	pointLightPosition.x = 0;
	pointLightPosition.z = 0;
	pointLightPosition.y = 00;
	pointLightColor.x = pointLightColor.y = pointLightColor.z = 1;
	gl::setPointLights(pointLightColor, pointLightPosition, 0.7);

	float depth = -60;
	
	vect3 regla;
	regla.x = -50;
	regla.y = 280;
	regla.z = 130;
	regla = !regla;

	//testing
	gl::STEPS = 1;
	//plastic.Texture = new modelImp::texture("barytest.bmp");
	//paper.Texture = new modelImp::texture("barytest.bmp");
	//trapo.Texture = new modelImp::texture("barytest.bmp");
	//counter.Texture = new modelImp::texture("barytest.bmp");
	//testing

	vect3 A, B, C;
	
	B.x = 10;
	B.y = 17;
	B.z = depth - 56;

	C.x = -10;
	C.y = B.y;
	C.z = depth - 55;

	A = B - regla * 10;

	gl::vols[0] = new triangle(A, B, C, plastic);

	B = A;
	B.x *= -1;


	gl::vols[1] = new triangle(A, B, C, plastic);
	
	B.x = 10;
	B.y = 18;
	B.z = depth-55;

	C.x = -10;
	C.y = B.y;
	C.z = depth-55;

	A = B - regla * 10;

	gl::vols[2] = new triangle(A, B, C, paper);

	B = A;
	B.x *= -1;

	gl::vols[3] = new triangle(C, B, A, paper);

	A.x = -27;
	A.y = 22;
	A.z = depth-74;

	B.x = -40;
	B.y = 20;
	B.z = depth-70;

	C.x = -25;
	C.y = A.y;
	C.z = depth - 60;

	gl::vols[4] = new triangle(A, B, C, trapo);

	C.x = -45;
	C.y = 22;
	C.z = depth - 72;

	A = B+(A - B)*0.8;

	gl::vols[5] = new triangle(C, B, A, trapo);

	A.x = -55;
	A.y = 20;
	A.z = depth-76;

	gl::vols[6] = new triangle(A, B, C, trapo);

	A.x += -160;
	A.y += 25;
	A.z = depth - 150;

	
	B.x = 120;
	B.y = -5;
	B.z = depth-20;

	C = A;
	C.x = 210;


	gl::vols[7] = new triangle(A, B, C, counter);

	C = B;
	C.x *= -1;

	gl::vols[8] = new triangle(C, B, A, counter);

	const int appleStartIndex = 9;
	vect3 center;
	center.x = 34;
	center.y = 24;
	center.z = depth - 63;
	float radio = 3;

	gl::vols[appleStartIndex] = new sphere(radio, center, greenapple);

	A = B;
	A.y = -100;

	gl::vols[appleStartIndex + 1] = new triangle(A, B, C, darkwood);

	B = A;
	B.x *= -1;
	
	gl::vols[appleStartIndex + 2] = new triangle(A, B, C, darkwood);
	
	center.x += 5;
	center.y += 1.1;
	center.z -= 2;
	gl::vols[appleStartIndex + 3] = new sphere(radio, center, greenapple);
	center.x += 5;
	center.y -= 2;
	center.z += 2;
	gl::vols[appleStartIndex + 4] = new sphere(radio, center, greenapple);

	center.x -= 0;
	center.y += 5;
	center.z -= 7;
	gl::vols[appleStartIndex + 5] = new sphere(radio, center, greenapple);
	center.x -= 4;
	center.y += 2;
	center.z -= 5;
	gl::vols[appleStartIndex + 6] = new sphere(radio, center, greenapple);
	center.x -= 6.7;
	center.y -= 2;
	center.z += 7;
	gl::vols[appleStartIndex + 7] = new sphere(radio, center, greenapple);

	center.x += 1;
	center.y += 3.8;
	center.z -= 7;
	gl::vols[appleStartIndex + 8] = new sphere(radio, center, greenapple);

	center.x += 3;
	center.y += 1;
	center.z -= 17;
	gl::vols[appleStartIndex + 14] = new sphere(radio, center, redapple);
	center.x -= 1;
	center.y -= 4;
	center.z += 7;
	gl::vols[appleStartIndex + 9] = new sphere(radio, center, redapple);
	center.x -= 0.2;
	center.y -= 4;
	center.z += 7;
	gl::vols[appleStartIndex + 10] = new sphere(radio, center, redapple);
	center.x -= 0.2;
	center.y -= 4;
	center.z += 7;
	gl::vols[appleStartIndex + 11] = new sphere(radio, center, redapple);
	center.x += 5;
	center.y -= 0.2;
	center.z += 1;
	gl::vols[appleStartIndex + 12] = new sphere(radio, center, redapple);
	center.x += 5;
	center.y -= 0.1;
	center.z += 1;
	gl::vols[appleStartIndex + 13] = new sphere(radio, center, redapple);
	


	render();
	glFinish("output.bmp", false);
	return 0;
}