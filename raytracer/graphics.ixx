#pragma warning(disable : 4996)
#include <iostream>
import ModelImporter;
import hb_math;
import volumes;




export module graphics;


using namespace modelImp;
using namespace hb_math;
using namespace volumes;


#define pixel_size  3
#define pi 3.1415926535897932



export namespace gl {



	class col3
	{
	public:
		unsigned char col[3];

		col3()
		{
			this->col[0] = 0;
			this->col[1] = 0;
			this->col[2] = 0;
		}

		col3(float r, float g, float b)
		{
			this->col[0] = (int)(clamp01(b) * 255);
			this->col[1] = (int)(clamp01(g) * 255);
			this->col[2] = (int)(clamp01(r) * 255);
		}

		unsigned char& operator[](int index)
		{
			switch (index)
			{
			case 0:
				return col[2];
			case 1:
				return col[1];
			case 2:
				return col[0];
			default:
				return col[2];
			}
		}
	};



	col3* clear_col = new col3();
	col3* draw_col = new col3();

	col3* WHITE = new col3(1, 1, 1);
	col3* BLACK = new col3();
	col3* maxZ;
	texture* active_textures[5];

	vect3 lightSources[10];
	vect3 lightColors[10];
	float lightIntensity[10];

	vect3 spotLights[10];
	vect3 spotLightColors[10];
	float spotLightIntensity[10];


	col3** frameBuffer;

	int width = 0;
	int height = 0;
	int vp_width = 0;
	int vp_height = 0;
	int vp_x = 0;
	int vp_y = 0;
	int vp_boundx = 0;
	int vp_boundy = 0;
	const int filesize = width * height * pixel_size;

	float fov = 60;
	sphere* spheres;
	int vol_size;

	vect3 CameraPosition;


	void glCreateViewPort(int x, int y, int width, int height)
	{
		vp_x = x;
		vp_y = y;
		vp_width = width;
		vp_height = height;
		vp_boundx = (vp_x + vp_width) > gl::width ? gl::width : (vp_x + vp_width);
		vp_boundy = (vp_y + vp_height) > gl::height ? gl::height : (vp_y + vp_height);
	}

	

	void glCreateWindow(int new_width = 1920, int new_height = 1080)
	{
	
		for (int i = 0; i < width; i++)
			delete[] frameBuffer[i];
		delete[] frameBuffer;

		width = new_width;
		height = new_height;
		glCreateViewPort(0, 0, new_width, new_height);
		frameBuffer = new col3 * [width];
		for (int i = 0; i < width; i++)
		{
			frameBuffer[i] = new col3[height];
		}

	}

	bool outOfBounds(int x, int y, int x0ff = 0, int y0ff = 0)
	{
		return x < x0ff + vp_x || y <  y0ff + vp_y || x > vp_boundx - 1 - x0ff || y > vp_boundy - 1 - y0ff;
	}


	void glFinish(const char* filename, bool end_exec = true)
	{
		unsigned char header[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		unsigned char headerinfo[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };

		*(int*)(header + 2) = filesize;

		*(int*)(headerinfo + 4) = width;

		*(int*)(headerinfo + 8) = height;

		FILE* dump = fopen(filename, "wb");

		fwrite(header, 1, 14, dump);
		fwrite(headerinfo, 1, 40, dump);

		int i, j = 0;
		while (j < height)
		{
			i = 0;
			while (i < width)
			{
				fwrite(&frameBuffer[i][j], 1, 3, dump);
				i++;
			}
			j++;
		}

		fclose(dump);
		for (int i = 0; i < width; i++)
			delete[] frameBuffer[i];
		delete[] frameBuffer;


		if (end_exec) exit(EXIT_SUCCESS);
	}


	void clear()
	{
		int i = vp_x, j;

		while (i < vp_boundx)
		{
			j = vp_y;
			while (j < vp_boundy)
			{
				if (outOfBounds(i, j)) { j++; continue; }
				frameBuffer[i][j] = *clear_col;
				j++;
			}
			i++;
		}
	}

	void gldrawPoint(int x, int y, col3* color = nullptr)
	{
		if (outOfBounds(x, y)) return;
		if (color != nullptr)
		{
			frameBuffer[x][y] = *color;
			return;
		}
		frameBuffer[x][y] = *draw_col;
	}


	void gldrawPoint(int x, int y, vect3* color = nullptr)
	{
		if (outOfBounds(x, y)) return;
		if (color != nullptr)
		{
			col3* col = new col3(color->x, color->y, color->z);
			frameBuffer[x][y] = *col;
			delete col;
			return;
		}
		frameBuffer[x][y] = *draw_col;
	}

	void set_draw_col(col3* new_col)
	{
		delete draw_col;
		draw_col = new_col;
	}

	void set_draw_col(const col3* new_col)
	{
		delete draw_col;
		draw_col = (col3*)new_col;
	}

	void set_clear_col(col3* new_col)
	{
		delete clear_col;
		clear_col = new_col;
	}

	void set_clear_col(const col3* new_col)
	{
		delete clear_col;
		clear_col = (col3*)new_col;
	}


	void glInit(int w = 1920, int h = 1080)
	{
		glCreateWindow(w, h);
		clear();
	}



	inline void barycentricCords(float ABC, vect2 A, vect2 B, vect2 C, vect2 P, vect3* out)
	{
		float u, v, w;



		if (ABC == 0)
		{
			out->x = -1;
			out->y = -1;
			out->z = -1;
			return;
		}

		u = ((B.y - C.y) * (P.x - C.x) + (C.x - B.x) * (P.y - C.y)) / ABC;

		v = ((C.y - A.y) * (P.x - C.x) + (A.x - C.x) * (P.y - C.y)) / ABC;

		w = 1 - u - v;
		out->x = u;
		out->y = v;
		out->z = w;
	}

	M4x4 createRotationMatrix(vect3 rotation)
	{
		M4x4  Rx, Ry, Rz;
		Rz = Ry = Rx = 'I';

		rotation = rotation * (pi / 180);
		Rx[1][1] = Rx[2][2] = cos(rotation.x);
		Rx[1][2] = sin(rotation.x);
		Rx[2][1] = -sin(rotation.x);
		Ry[0][0] = Ry[2][2] = cos(rotation.y);
		Ry[2][0] = sin(rotation.y);
		Ry[0][2] = -sin(rotation.y);
		Rz[0][0] = Rz[1][1] = cos(rotation.z);
		Rz[0][1] = sin(rotation.z);
		Rz[1][0] = -sin(rotation.z);

		return Rx * Ry * Rz;
	}

	M4x4 createObjectMatrix(vect3 scale, vect3 translate, vect3 rotation)
	{
		M4x4 Mt, Ms, Mr;
		Ms = Mt = 'I';
		Mt[0][3] = translate.x;
		Mt[1][3] = translate.y;
		Mt[2][3] = translate.z;

		Ms[0][0] = scale.x;
		Ms[1][1] = scale.y;
		Ms[2][2] = scale.z;

		Mr = createRotationMatrix(rotation);

		return  Mt * Mr * Ms;
	}

	vect3 transform(vect3 V, M4x4 Matrix)
	{
		vect4 result;

		result = V;
		result = Matrix * result;
		result = result * (1 / result.w);

		vect3 vf;
		vf.x = result.x;
		vf.y = result.y;
		vf.z = result.z;

		return vf;
	}

	vect3 dirTransform(vect3 V, M4x4 Matrix)
	{
		vect4 result;

		result = V;
		result.w = 0;
		result = Matrix * result;


		vect3 vf;
		vf.x = result.x;
		vf.y = result.y;
		vf.z = result.z;

		return vf;
	}


	vect3* raycast(vect3 dir) 
	{
		float depth = FLT_MAX;
		vect3* diffuse = nullptr;
		intersect* hit;
		int i = 0;
		while (i < vol_size)
		{
			hit = hitSphere(&spheres[i], &CameraPosition, &dir);
			if (hit && hit->distance < depth) 
			{
				depth = hit->distance;
				diffuse = &spheres[i].mat.diffuse;
				delete hit;
			}
			
			i++;
		}
		return diffuse;
	}


	void render()
	{
		vect3 dir;
		dir.z = -1;
		int i = 0, j;
		float  t;
		while (i < width)
		{
			j = 0;
			while (j < height)
			{
				dir.x = 2 * ((i + 0.5f) / (float)width) - 1;
				dir.y = 2 * ((j + 0.5f) / (float)height) - 1;

				t = tan(fov * pi / 360);
				dir.x *= t * (float)width / (float)height;
				dir.y *= t;

			    gldrawPoint(i, j, raycast(!dir));

				j++;
			}
			i++;
		}
	}

}