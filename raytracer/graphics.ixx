#pragma warning(disable : 4996)
#include <iostream>
#include<math.h>
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

	//ambient light
	vect3* ambientColor;
	//directional light
	vect3* directionalColor;
	vect3 dirLight;
	//point Lights
	vect3 pointLightsCol[10];
	vect3 pointLightsPos[10];
	int pointLI = 0;
	int MAX_RECURSION = 3;
	texture* enviromentMap;

	void setAmbientLight(vect3 color, float intensity) 
	{
		if (ambientColor)
			delete ambientColor;
		ambientColor = new vect3();
		*ambientColor = color * intensity;
	}

	void setDirLight(vect3 color, vect3 direction, float intensity)
	{
		if (directionalColor)
			delete directionalColor;
		directionalColor = new vect3();
		*directionalColor = color * intensity;
		dirLight = !(direction * -1);
	}

	void setPointLights(vect3 color, vect3 position , float intesity) 
	{
		pointLightsCol[pointLI] = color * intesity;
		pointLightsPos[pointLI] = position;
		pointLI++;
	}


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
			delete color;
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



	



	bool pointShadowTest(int PLindex, vect3 point, void* check_for_object) 
	{
		int i = 0;
		vect3 dir = (!(pointLightsPos[PLindex] - point));
		intersect* hit = nullptr;
		while (i < vol_size)
		{
			if (hit)
				delete hit;
			//if it hits any object other than the object we want to know if it gets ocluded
			hit = hitSphere(&spheres[i], &CameraPosition, &dir);
			if (hit && hit->object != check_for_object)
			{
				delete hit;
				return true;
			}
			i++;
		}
		return false;
	}

	bool dirShadowTest(vect3 orig, vect3 dir, void* ignore)
	{
		int i = 0;
		intersect* hit;
		while (i < vol_size)
		{
			if (ignore == &spheres[i])
			{
				i++;
				continue;
			}
			hit = hitSphere(&spheres[i], &CameraPosition, &dir);
			if (hit)
			{
				delete hit;
				return true;
			}
			i++;
		}
		return false;
	}

	vect3* raycast(vect3 dir, vect3* orig = nullptr, void* ignore = nullptr, int recursion = 0) 
	{
		float depth = FLT_MAX;
		vect3* diffuse = nullptr;
		intersect* hit = nullptr;
		intersect* hitCheck = nullptr;
		vect3 aLightColor;
		vect3 dLightColor;
		vect3 pLightColor;
		vect3 specColor;
		vect3 lightColoration;
		vect3 viewDir;
		material Mat;

		if (recursion >= MAX_RECURSION)
			if (enviromentMap)
			{
				vect3* envColor = new vect3();
				enviromentMap->getColor(dir, envColor);
				return envColor;
			}
			else
				return nullptr;

		float intensity;
		float specIntensity;
		int i = 0;
		while (i < vol_size)
		{	
			if (ignore == &spheres[i])
			{
				i++;
				continue;
			}
			hitCheck = hitSphere(&spheres[i], &CameraPosition, &dir);
			if (hitCheck)
			{
				if (hitCheck->distance < depth) 
				{
					depth = hitCheck->distance;
					if (!hit)
						hit = new intersect();
					*hit = *hitCheck;
				}
				delete hitCheck; 
			}
			i++;
		}
		i = 0;
		if (hit) 
		{
			//std::cout << "leak check" << std::endl;
			diffuse = new vect3();
			Mat = hit->Mat;
			*diffuse = ((sphere*)hit->object)->mat.diffuse;
			viewDir = !(CameraPosition - hit->point);
			float shadow = 1;

			if (ambientColor)
				aLightColor = *ambientColor;

			if (directionalColor)
			{
				intensity = clamp01(dirLight ^ hit->normal);
				specIntensity = clamp01(reflection(hit->normal, dirLight) ^ viewDir);
				specIntensity = pow(specIntensity, Mat.specular);

				if (intensity > 0 && dirShadowTest(hit->point, dirLight*-1, hit->object))
					shadow = 0;
				
				specColor = specColor + *directionalColor * specIntensity * shadow;
				dLightColor = *directionalColor * intensity * shadow;
			}

			while (i < pointLI)
			{
				shadow = 1;
				vect3 pLightDir = (pointLightsPos[i] - hit->point);
				pLightDir = !pLightDir;
				intensity = clamp01(pLightDir ^ hit->normal);
				specIntensity = clamp01(reflection(hit->normal, pLightDir) ^ viewDir);
				specIntensity = pow(specIntensity, Mat.specular);

				if (pointShadowTest(i, hit->point, hit->object))
					shadow = 0;

				specColor = specColor + pointLightsCol[i] * specIntensity * shadow;
				pLightColor = pLightColor + pointLightsCol[i] * intensity * shadow;
				i++;
			}

			if (Mat.type == OPAQUE)
				lightColoration = aLightColor + dLightColor + pLightColor + specColor;
			
			if (Mat.type == REFLECTIVE) 
			{
				vect3 reflect;
				reflect = reflection(hit->normal, dir * -1);
				vect3* reflect_color = raycast(reflect, &hit->point, hit->object, recursion + 1);
				if (reflect_color)
				{
					lightColoration = *reflect_color + specColor;
					delete reflect_color;
				}
			}
			if (Mat.type == TRANSPARENT)
			{
				//TODO
			}
			diffuse->x = clamp01(diffuse->x * lightColoration.x);
			diffuse->y = clamp01(diffuse->y * lightColoration.y);
			diffuse->z = clamp01(diffuse->z * lightColoration.z);
			delete hit;
		}
		else
			if (enviromentMap)
			{
				vect3* envColor = new vect3();
				enviromentMap->getColor(dir, envColor);
				return envColor;
			}
			else
				return nullptr;
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