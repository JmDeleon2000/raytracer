#include <fstream>
#include <string>
#include <iostream>
#include <vector>

export module ModelImporter;
import hb_math;

using namespace std;
using namespace hb_math;


#define importer_debug false

export namespace modelImp
{



	struct face
	{
	public:
		int size;
		int* data;
	};

	class obj
	{
	public:
		vect3* v;
		vect3* uvs;
		vect3* n;
		face* f;
		int v_size, uv_size, n_size, f_size;



	public:
		obj(const char* file)
		{
			vector<vect3> v_temp;
			vector<vect3> uvs_temp;
			vector<vect3> n_temp;
			vector<face> f_temp;

			ifstream stream;
			stream.open(file);
			string line;
			vect3 temp;

			while (getline(stream, line))
			{
				if (line[0] == 'v' && line[1] != 't' && line[1] != 'n') //vert
				{
#if importer_debug
					cout << line << endl;
#endif
					line.erase(0, 2);
					double x = (double)atof(line.substr(0, line.find(" ")).c_str());
					line.erase(0, line.find(" ") + 1);
					double y = (double)atof(line.substr(0, line.find(" ")).c_str());
					line.erase(0, line.find(" ") + 1);
					double z = (double)atof(line.substr(0, line.find(" ")).c_str());
#if importer_debug
					cout << x << " " << y << " " << z << endl;
#endif // importer_debug
					temp.x = x;
					temp.y = y;
					temp.z = z;

					v_temp.push_back(temp);
					continue;
				}
				if (line[0] == 'v' && line[1] == 't') //uv
				{
#if importer_debug
					cout << line << endl;
#endif
					line.erase(0, 3);
					double x = (double)atof(line.substr(0, line.find(" ")).c_str());
					line.erase(0, line.find(" ") + 1);
					double y = (double)atof(line.substr(0, line.find(" ")).c_str());
					line.erase(0, line.find(" ") + 1);
					double z = (double)atof(line.substr(0, line.find(" ")).c_str());

#if importer_debug
					cout << x << " " << y << " " << z << endl;
#endif // importer_debug
					temp.x = x;
					temp.y = y;
					temp.z = z;

					uvs_temp.push_back(temp);
					continue;
				}
				if (line[0] == 'v' && line[1] == 'n') //normals
				{
#if importer_debug
					cout << line << endl;
#endif
					line.erase(0, 3);
					double x = (double)atof(line.substr(0, line.find(" ")).c_str());
					line.erase(0, line.find(" ") + 1);
					double y = (double)atof(line.substr(0, line.find(" ")).c_str());
					line.erase(0, line.find(" ") + 1);
					double z = (double)atof(line.substr(0, line.find(" ")).c_str());

#if importer_debug
					cout << x << " " << y << " " << z << endl;
#endif // importer_debug
					temp.x = x;
					temp.y = y;
					temp.z = z;

					n_temp.push_back(temp);
					continue;
				}
				if (line[0] == 'f') //faces
				{

					face new_face;
#if importer_debug
					cout << line << endl;
#endif

					line.erase(0, 2);

					vector<int> k;
					string sub;
					while (line.find("/") != string::npos)
					{
						sub = line.substr(0, line.find(" "));
						while (!sub.empty())
						{
							string x = sub.substr(0, sub.find("/"));
							k.push_back((int)atoi(x.c_str()));
							sub.erase(0, x.length() + 1);
						}
						if (line.find(" ") == string::npos)
							break;
						line.erase(0, line.find(" ") + 1);
					}
					k.shrink_to_fit();



					new_face.data = new int[k.size()];
					std::memcpy(new_face.data, k.data(), k.size() * sizeof(int));
					new_face.size = k.size();
#if importer_debug

					cout << new_face.data[0] << " " << new_face.data[1] << " " << new_face.data[2] << endl;
					cout << new_face.data[3] << " " << new_face.data[4] << " " << new_face.data[5] << endl;
					cout << new_face.data[6] << " " << new_face.data[7] << " " << new_face.data[8] << endl;
#endif
					f_temp.push_back(new_face);
				}
			}

			v_temp.shrink_to_fit();
			uvs_temp.shrink_to_fit();
			n_temp.shrink_to_fit();
			f_temp.shrink_to_fit();
			v_size = v_temp.size();
			uv_size = uvs_temp.size();
			n_size = n_temp.size();
			f_size = f_temp.size();

			v = new vect3[v_temp.size()];
			std::memcpy(v, v_temp.data(), v_temp.size() * sizeof(vect3));
			v_temp.clear();
			v_temp.shrink_to_fit();

			uvs = new vect3[uvs_temp.size()];
			std::memcpy(uvs, uvs_temp.data(), uvs_temp.size() * sizeof(vect3));
			uvs_temp.clear();
			uvs_temp.shrink_to_fit();

			n = new vect3[n_temp.size()];
			std::memcpy(n, n_temp.data(), n_temp.size() * sizeof(vect3));
			n_temp.clear();
			n_temp.shrink_to_fit();

			f = new face[f_temp.size()];
			std::memcpy(f, f_temp.data(), f_temp.size() * sizeof(face));
			f_temp.clear();
			f_temp.shrink_to_fit();
			cout << "Loaded " << f_size << " polygons" << endl;

#if importer_debug
			cout << v[v_size - 1].x << " " << v[v_size - 1].y << " " << v[v_size - 1].z << endl;
			cout << uvs[uv_size - 1].x << " " << uvs[uv_size - 1].y << " " << uvs[uv_size - 1].z << endl;
			cout << n[n_size - 1].x << " " << n[n_size - 1].y << " " << n[n_size - 1].z << endl;
			int i = 0;
			cout << "f_size: " << f_size << endl;
			cout << f[f_size - 1].size << endl;
			while (i < f[f_size - 1].size)
			{
				cout << f[f_size - 1].data[i] << "/";
				i++;
			}
			cout << endl;
#endif


			stream.close();
		}



		~obj()
		{
			delete[] v;
			delete[] uvs;
			delete[] n;
			delete[] f;
		}
	};

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

	};

	class texture
	{
	public:
		int width = 0;
		int height = 0;
		col3** image;

		texture() 
		{
		}

		texture(const char* filename)
		{
			int headerSize;
			ifstream text;
			text.open(filename, ios::binary);

			text.seekg(10);
			text.read((char*)&headerSize, 4);
			text.seekg(18);
			text.read((char*)&width, 4);
			text.read((char*)&height, 4);
			text.seekg(headerSize);
			image = new col3 * [height];

			int i = 0, j;
			while (i < width)
			{
				j = 0;
				image[i] = new col3[width];
				while (j < height)
				{
					//image[i][j] = *new col3();
					text.read((char*)image[i][j].col, 3);
					j++;
				}
				i++;
			}
			text.close();
		}

		void getColor(float x, float y, vect3* out)
		{
			if (!out)return;
			if (x < 0)
			{
				x = ((int)x)*-1+x;
			}
			if (x >= 1)
			{
				x = x-(int)x;
			}
			if (y < 0)
			{
				y = ((int)y) * -1 + y;
			}
			if (y >= 1)
			{
				y = y-(int)y;
			}
			int j = x * width;
			int i = y * height;

			out->z = (float)image[i][j].col[0]/255;
			out->y = (float)image[i][j].col[1]/255;
			out->x = (float)image[i][j].col[2]/255;
			
		}
	};
}