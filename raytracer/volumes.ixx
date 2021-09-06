#include <math.h>
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
		float radius;
		vect3 center;
		material mat;
	};

	struct intersect
	{
		float distance = 0;
	};

	intersect* hitSphere(sphere* a, vect3* orig, vect3* dir) 
	{
		intersect* result;
		vect3 L = a->center - *orig;
		float tca_sqr = L ^ *dir;
		tca_sqr *= tca_sqr;
		float l_sqr = L.x * L.x + L.y * L.y + L.z * L.z;
		float d_sqr = sqrtf(l_sqr - tca_sqr);

		if (d_sqr >  a->radius) return nullptr;

		float thc_sqr = a->radius  - d_sqr;
		float t0 = (L ^ *dir) - thc_sqr;
		float t1 = (L ^ *dir) +  thc_sqr;

		if (t0 < 0)
			t0 = t1;

		if (t0 < 0)
			return nullptr;
		result = new intersect();
		result->distance = t0;
		
		return result;

	}

};