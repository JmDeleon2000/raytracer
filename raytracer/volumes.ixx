#include <math.h>
#include<iostream>
import hb_math;


export module volumes;

using namespace hb_math;

export namespace volumes
{
	struct material
	{
		vect3 diffuse;
	};

	struct sphere
	{
		double radius;
		vect3 center;
		material mat;
	};

	struct intersect
	{
		double distance = 0;
	};

	intersect* hitSphere(sphere* a, vect3* orig, vect3* dir) 
	{
		intersect* result;
		vect3 L = a->center - *orig;
		double tca= L ^ *dir;
		
		double l_sqr = (L.x * L.x + L.y * L.y + L.z * L.z);
		double d = (l_sqr - tca*tca);

//		if (d < 0) return nullptr;
		//std::cout << d << std::endl;
		if (d >  a->radius* a->radius) return nullptr;

		double thc = sqrt(a->radius * a->radius - d);
		double t0 = tca - thc;
		double t1 = tca + thc;

		if (t0 < 0)
			t0 = t1;

		if (t0 < 0)
			return nullptr;
		result = new intersect();
		result->distance = t0;
		
		return result;

	}

};