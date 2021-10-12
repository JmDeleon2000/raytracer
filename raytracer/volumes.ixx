#include <math.h>
#include<iostream>
import hb_math;

import ModelImporter;
export module volumes;

using namespace hb_math;

export namespace volumes
{
	enum materialType
	{
		OPAQUE = 0,
		REFLECTIVE,
		TRANSPARENT
	};
	struct material
	{
		vect3 diffuse;
		double specular = 1;
		float ior = 1;
		int type;
		modelImp::texture* Texture = nullptr;
	};

	
	

	struct intersect
	{
		double distance = 0;
		vect3 point;
		vect3 normal;
		void* object;
		material Mat;
		float texcordx = 0;
		float texcordy = 0;
	};

	class volume
	{
		public :
			material mat;
		public:
			virtual intersect* ray_hit(vect3* orig, vect3* dir) { return nullptr; }
	};

	class sphere : public volume
	{
		public:
			double radius;
			vect3 center;
		

		public:
			sphere() 
			{

			}

			sphere(double radius, vect3 center, material mat) 
			{
				this->radius = radius;
				this->center = center;
				this->mat = mat;
			}

			intersect* ray_hit(vect3* orig, vect3* dir)
			{
				intersect* result;
				vect3 L = this->center - *orig;
				double tca = L ^ *dir;

				double l_sqr = (L.x * L.x + L.y * L.y + L.z * L.z);
				double d = (l_sqr - tca * tca);

				//		if (d < 0) return nullptr;
						//std::cout << d << std::endl;
				if (d > this->radius * this->radius) return nullptr;

				//sqrt represents a precision risk
				double thc = sqrt(this->radius * this->radius - d);
				double t0 = tca - thc;
				double t1 = tca + thc;

				if (t0 < 0)
					t0 = t1;

				if (t0 < 0)
					return nullptr;

				result = new intersect();
				result->distance = t0;
				result->point = *orig + (*dir * t0);
				result->normal = !(result->point - this->center);
				result->object = this;
				result->Mat = this->mat;
				return result;
			}
	};

	class plane : public volume 
	{
	public:
		vect3 point;
		vect3 normal;
		vect3 wrapDirection;
		vect3 wrapDirectionY;

	public:
		plane() {}
		plane(vect3 point, vect3 normal, material mat)
		{
			this->point = point;
			this->normal = !normal;
			this->mat = mat;
			vect3 orig;
			vect3 dir = !(point * 1.5);
			float denom = dir ^ normal;
			float t = ((point - orig) ^ normal) / denom;
			vect3 hit = orig + dir * t;

			wrapDirection = (point - hit);
			wrapDirectionY = !(wrapDirection * normal);
			wrapDirection = !wrapDirection;
		}
		plane(vect3 point, vect3 normal)
		{
			this->point = point;
			this->normal = !normal;
		}
		intersect* ray_hit(vect3* orig, vect3* dir) 
		{ 
			float t;
			float denom = *dir ^ normal;

			if (denom > 0.0001)
			{
				t = ((point - *orig) ^ normal) / denom;
				if (t > 0)
				{
					vect3 hit = *orig + *dir * t;

					intersect* result = new intersect();
					result->point = hit;
					result->distance = t;
					result->Mat = mat;
					result->object = this;
					result->normal = normal;
					if (mat.Texture)
					{
						vect3 wrapRef = point - hit;
						result->texcordx = wrapRef ^ wrapDirection;
						result->texcordy = wrapRef ^ wrapDirectionY;
					}
					return result;
				}
			}
			return nullptr; 
		}
	};

	class triangle : public volume 
	{
	public:
		vect3 A;
		vect3 B;
		vect3 C;
		vect3 normal;
		material mat;

	private:
		vect3 inner_plane_X_dir;
		vect3 inner_plane_Y_dir;
		vect2 ip_A;
		vect2 ip_B;
		vect2 ip_C;
		float Area = 0;


	public:
		triangle(vect3 A, vect3 B, vect3 C, material mat) 
		{
			this->A = A;
			this->B = B;
			this->C = C;
			this->mat = mat;

			vect3 AB = (B - A);
			vect3 AC = (C - A);
			normal = (AB * AC);
			inner_plane_Y_dir = AB;
			inner_plane_X_dir = !(inner_plane_Y_dir * normal);
			normal = !normal;
			inner_plane_Y_dir = !inner_plane_Y_dir;

			ip_B.y = (AB ^ inner_plane_Y_dir);

			ip_C.x = (AC ^ inner_plane_X_dir);
			ip_C.y = (AC ^ inner_plane_Y_dir);
			
			Area = ((ip_B.y - ip_C.y) * (ip_A.x - ip_C.x) + (ip_C.x - ip_B.x) * (ip_A.y - ip_C.y));
		}

		void barycentricCords(vect2 P, vect3* out)
		{
			float u, v, w;

			if (Area == 0)
			{
				out->x = -1;
				out->y = -1;
				out->z = -1;
				return;
			}

			u = ((ip_B.y - ip_C.y) * (P.x - ip_C.x) + (ip_C.x - ip_B.x) * (P.y - ip_C.y)) / Area;

			v = ((ip_C.y - ip_A.y) * (P.x - ip_C.x) + (ip_A.x - ip_C.x) * (P.y - ip_C.y)) / Area;

			w = 1 - u - v;
			out->x = u;
			out->y = v;
			out->z = w;
		}


		intersect* ray_hit(vect3* orig, vect3* dir)
		{
			float t;
			float denom = *dir ^ normal;
#define nomralFix false
#if normalFix
			bool flip = false;


			if (denom < 0) 
			{
				flip = true;
			}
#endif
			if (abs(denom) > 0.0001)
			{
				t = ((A - *orig) ^ normal) / denom;
				vect3 uvw;
				if (t > 0)
				{
					vect3 hit = *orig + *dir * t;

					vect2 hit_plane_cords;
					hit_plane_cords.x = (hit - A) ^ inner_plane_X_dir;
					hit_plane_cords.y = (hit - A) ^ inner_plane_Y_dir;

					barycentricCords(hit_plane_cords, &uvw);
					if (uvw.x < 0 || uvw.y < 0 || uvw.z < 0)
						return nullptr;

					intersect* result = new intersect();
					result->point = hit;
					result->distance = t;
					result->Mat = mat;
					result->object = this;
#if normalFix
					result->normal = flip ? normal * -1 : normal;
#endif
					result->normal =  normal;
					result->texcordx = uvw.y;
					result->texcordy = uvw.z;
					
					return result;
				}
			}
			return nullptr;
		}
	};

	class Face : public volume 
	{
		
	public:
		vect3 A;
		vect3 B;
		vect3 C;
		vect3 normal;
		vect2 A_uv;
		vect2 B_uv;
		vect2 C_uv;
		vect3 A_normal;
		vect3 B_normal;
		vect3 C_normal;

	private:
		vect3 inner_plane_X_dir;
		vect3 inner_plane_Y_dir;
		vect2 ip_A;
		vect2 ip_B;
		vect2 ip_C;
		float Area = 0;


	public:
		Face() {}
		Face(vect3 A, vect3 B, vect3 C, 
			vect2 A_uv, vect2 B_uv, vect2 C_uv, 
			vect3 A_normal, vect3 B_normal, vect3 C_normal)
		{
			this->A = A;
			this->B = B;
			this->C = C;
			this->A_uv = A_uv;
			this->B_uv = B_uv;
			this->C_uv = C_uv;
			this->A_normal = A_normal;
			this->B_normal = B_normal;
			this->C_normal = C_normal;

			vect3 AB = (B - A);
			vect3 AC = (C - A);
			normal = (AB * AC);
			inner_plane_Y_dir = AB;
			inner_plane_X_dir = !(inner_plane_Y_dir * normal);
			normal = !normal;
			inner_plane_Y_dir = !inner_plane_Y_dir;

			ip_B.y = (AB ^ inner_plane_Y_dir);

			ip_C.x = (AC ^ inner_plane_X_dir);
			ip_C.y = (AC ^ inner_plane_Y_dir);

			Area = ((ip_B.y - ip_C.y) * (ip_A.x - ip_C.x) + (ip_C.x - ip_B.x) * (ip_A.y - ip_C.y));
		}

		void barycentricCords(vect2 P, vect3* out)
		{
			float u, v, w;

			if (Area == 0)
			{
				out->x = -1;
				out->y = -1;
				out->z = -1;
				return;
			}

			u = ((ip_B.y - ip_C.y) * (P.x - ip_C.x) + (ip_C.x - ip_B.x) * (P.y - ip_C.y)) / Area;

			v = ((ip_C.y - ip_A.y) * (P.x - ip_C.x) + (ip_A.x - ip_C.x) * (P.y - ip_C.y)) / Area;

			w = 1 - u - v;
			out->x = u;
			out->y = v;
			out->z = w;
		}


		intersect* ray_hit(vect3* orig, vect3* dir)
		{
			float t;
			float denom = abs(*dir ^ normal);

			if (denom > 0.0001)
			{
				t = ((A - *orig) ^ normal) / denom;
				vect3 uvw;
				if (t > 0)
				{
					vect3 hit = *orig + *dir * t;

					vect2 hit_plane_cords;
					hit_plane_cords.x = (hit - A) ^ inner_plane_X_dir;
					hit_plane_cords.y = (hit - A) ^ inner_plane_Y_dir;

					barycentricCords(hit_plane_cords, &uvw);
					if (uvw.x < 0 || uvw.y < 0 || uvw.z < 0)
						return nullptr;

					intersect* result = new intersect();
					result->point = hit;
					result->distance = t;
					result->normal = A_normal * uvw.x + B_normal * uvw.y + C_normal * uvw.z;
					result->texcordx = A_uv.x * uvw.x + B_uv.x * uvw.y + C_uv.x * uvw.z;
					result->texcordy = A_uv.y * uvw.x + B_uv.y * uvw.y + C_uv.y * uvw.z;

					return result;
				}
			}
			return nullptr;
		}
	};

	class AABB : public volume 
	{//TODO
	public:
		plane planes[6];
		vect3 minBound;
		vect3 maxBound;

	public:
		AABB() {}
		AABB(vect3 size, vect3 position)
		{
			this->mat = mat;
			minBound = position - size * 0.5;
			maxBound = position + size * 0.5;
			vect3 normal;
			normal.z = -1;
			new (&planes[0]) plane(maxBound, normal);
			normal.z = 0;
			normal.x = 1;
			new (&planes[1]) plane(maxBound, normal);
			normal.x = 0;
			normal.y = 1;
			new (&planes[2]) plane(maxBound, normal);
			normal.y = 0;
			normal.z = -1;
			new (&planes[3]) plane(minBound, normal);
			normal.z = 0;
			normal.x = -1;
			new (&planes[4]) plane(minBound, normal);
			normal.x = 0;
			normal.y = -1;
			new (&planes[5]) plane(minBound, normal);
			vect3 bias;
			bias.x = bias.y = bias.z = 1;
			this->minBound = minBound + bias * -1;
			this->maxBound = maxBound + bias;//test
		}

		AABB(vect3 size, vect3 position, material mat)
		{
			this->mat = mat;
			minBound = position - size * 0.5;
			maxBound = position + size * 0.5;
			vect3 normal;
			normal.z = -1;
			new (&planes[0]) plane(maxBound, normal);
			normal.z = 0;
			normal.x = 1;
			new (&planes[1]) plane(maxBound, normal);
			normal.x = 0;
			normal.y = 1;
			new (&planes[2]) plane(maxBound, normal);
			normal.y = 0;
			normal.z = -1;
			new (&planes[3]) plane(minBound, normal);
			normal.z = 0;
			normal.x = -1;
			new (&planes[4]) plane(minBound, normal);
			normal.x = 0;
			normal.y = -1;
			new (&planes[5]) plane(minBound, normal);
			vect3 bias;
			bias.x = bias.y = bias.z = 1;
			this->minBound = minBound + bias * -1;
			this->maxBound = maxBound + bias;//test
		}

		intersect* ray_hit(vect3 *orig, vect3 *dir) 
		{
			intersect *result;
			int i = 0;
			while (i < 6)
			{
				result = planes[i].ray_hit(orig, dir);
				if (result) goto satisfactory;
				i++;
			}
			return nullptr;
		satisfactory:
			//std::cout << i << std::endl;
			if (minBound.x <= result->point.x)
				if(minBound.y <= result->point.y)
				if(minBound.z <= result->point.z)
				if(maxBound.x >= result->point.x)
				if(maxBound.y >= result->point.y)
				if(maxBound.z >= result->point.z)
				{
					result->normal.z *= -1;
					return result;
				}
					
			delete result;
			return nullptr;
		}

	};

	class Model : public volume
	{
	public:
		volume *boundingVolume;
		Face* faces;
		int fcount;

	public:
		Model(modelImp::obj *source, vect3 scale, vect3 translate, material mat) 
		{
			this->mat = mat;
			int i = 0;
			vect3 minBound;
			minBound.x = minBound.y = minBound.z = FLT_MAX;
			vect3 maxBound;
			maxBound.x = maxBound.y = maxBound.z = -FLT_MAX;
			vect3 v[3], vn[3];
			vect2 vt[3];
			faces = new Face[source->f_size];
			while (i < source->f_size)
			{
				v[0] = (source->v[source->f[i].data[0] - 1] * scale) + translate;
				v[1] = (source->v[source->f[i].data[3] - 1] * scale) + translate;
				v[2] = (source->v[source->f[i].data[6] - 1] * scale) + translate;

				if (minBound.x > v[0].x) minBound.x = v[0].x;
				if (minBound.x > v[1].x) minBound.x = v[1].x;
				if (minBound.x > v[2].x) minBound.x = v[2].x;
				if (minBound.y > v[0].x) minBound.y = v[0].y;
				if (minBound.y > v[1].x) minBound.y = v[1].y;
				if (minBound.y > v[2].x) minBound.y = v[2].y;
				if (minBound.z > v[0].x) minBound.z = v[0].z;
				if (minBound.z > v[1].x) minBound.z = v[1].z;
				if (minBound.z > v[2].x) minBound.z = v[2].z;

				if (maxBound.x < v[0].x) maxBound.x = v[0].x;
				if (maxBound.x < v[1].x) maxBound.x = v[1].x;
				if (maxBound.x < v[2].x) maxBound.x = v[2].x;
				if (maxBound.y < v[0].x) maxBound.y = v[0].y;
				if (maxBound.y < v[1].x) maxBound.y = v[1].y;
				if (maxBound.y < v[2].x) maxBound.y = v[2].y;
				if (maxBound.z < v[0].x) maxBound.z = v[0].z;
				if (maxBound.z < v[1].x) maxBound.z = v[1].z;
				if (maxBound.z < v[2].x) maxBound.z = v[2].z;


				vt[0].x = source->uvs[source->f[i].data[1] - 1].x;
				vt[0].y = source->uvs[source->f[i].data[1] - 1].y;
				vt[1].x = source->uvs[source->f[i].data[4] - 1].x;
				vt[1].y = source->uvs[source->f[i].data[4] - 1].y;
				vt[2].x = source->uvs[source->f[i].data[7] - 1].x;
				vt[2].y = source->uvs[source->f[i].data[7] - 1].y;

				vn[0] = source->n[source->f[i].data[2] - 1];
				vn[1] = source->n[source->f[i].data[5] - 1];
				vn[2] = source->n[source->f[i].data[8] - 1];

				new (&faces[i]) Face(v[0], v[1], v[2],
									vt[0], vt[1], vt[2], 
									vn[0], vn[1], vn[2]);
				i++;
			}
			fcount = source->f_size;
			vect3 bound = maxBound - minBound;
			float radio = bound.x + bound.x + bound.y + bound.y + bound.z + bound.z;
			radio = sqrtf(radio);
			boundingVolume = new sphere(radio, translate, mat);
		}

		intersect* ray_hit(vect3* orig, vect3* dir) 
		{
			intersect* bbtest = boundingVolume->ray_hit(orig, dir);
			if (bbtest)
			{
				delete bbtest;
				int i = 0;
				intersect* result = nullptr, * hit;
				float depth = -FLT_MAX;
				while (i < fcount)
				{
					hit = faces[i].ray_hit(orig, dir);
					if (hit) 
					{
						if (depth < hit->distance) 
						{
							depth = hit->distance;
							if (!result) result = new intersect();
							result->distance = hit->distance;
							result->Mat = this->mat;
							result->normal = hit->normal;
							result->object = this;
							result->point = hit->point;
							result->texcordx = hit->texcordx;
							result->texcordy = hit->texcordy;
							
						}
						delete hit; 
					}
					i++;
				}
			}
			return nullptr;
		}
	};

	class cylinder : public volume
	{
	public:
		float radius;
		float height;
		vect3 center;

	public:
		cylinder() {}
		cylinder(vect3 center, float radius, float height) 
		{
			this->radius = radius;
			this->height = height;
			this->center = center;
		}

		intersect* ray_hit(vect3* orig, vect3* dir) 
		{

		

			vect2 center2D, orig2D, dir2D;
			center2D.x = center.x;
			center2D.y = center.z;
			orig2D.x = orig->x;
			orig2D.y = orig->z;
			dir2D.x = dir->x;
			dir2D.y = dir->z;

			float t = 0;

			intersect* result;
			vect2 L = center2D - orig2D;
			double tca = L ^ dir2D;

			double l_sqr = (L.x * L.x + L.y * L.y);
			double d = (l_sqr - tca * tca);

			//		if (d < 0) return nullptr;
					//std::cout << d << std::endl;
			if (d > this->radius * this->radius) return nullptr;

			//sqrt represents a precision risk
			double thc = sqrt(this->radius * this->radius - d);
			double t0 = tca - thc;
			double t1 = tca + thc;

			if (t0 < 0)
				t0 = t1;

			if (t0 < 0)
				return nullptr;
			vect3 point = *orig + (*dir * t0);
			if (point.y - center.y > height)
				return nullptr;

			result = new intersect();
			result->distance = t0;
			result->point = *orig + (*dir * t0);
			vect3 n = (result->point - this->center);
			n.y = 0;
			result->normal = !n;
			result->object = this;
			result->Mat = this->mat;
			return result;

			//keep it circle
			//hope
			return nullptr;
		}
	};
};