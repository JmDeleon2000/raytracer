export module hb_math;

export namespace hb_math
{
	struct vect2
	{
		float x = 0;
		float y = 0;
		float& operator[](int index)
		{
			switch (index)
			{
			case 0:
				return x;
			case 1:
				return y;
			default:
				return x;
			}
		}
	};

	struct vect3 {

		float  x = 0, y = 0, z = 0;
		float& operator[](int index)
		{
			switch (index)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			default:
				return x;
			}
		}

	};

	float clamp01(float x)
	{
		return x < 0.0f ? 0.0f : x > 1.0f ? 1.0f : x;
	}

	struct vect4
	{
		float x = 0, y = 0, z = 0, w = 0;

		float& operator[](int index)
		{
			switch (index)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			default:
				return x;
			}
		}

		vect4 operator=(const vect4& a) 
		{
			x = a.x;
			y = a.y;
			z = a.z;
			w = a.w;
			return *this;
		}

		vect4 operator=(const vect3& a)
		{
			x = a.x;
			y = a.y;
			z = a.z;
			w = 1;
			return *this;
		}
	};

	struct M4x4
	{
		float m[4][4];
		float* operator[](int index)
		{
			return m[index];
		}



		M4x4 operator=(const char& mode) 
		{
			if (mode == 'I') //identity
			{
				int i = 0, j;
				while (i < 4)
				{
					j = 0;
					while (j < 4)
					{
						m[i][j] = 0;
						j++;
					}
					m[i][i] = 1;
					i++;
				}
			}
			return *this;
		}
	};

	
	vect4 operator*(const M4x4& a, const vect4& b)
	{
		vect4 result;

		int i = 0;
		while (i < 16)
		{
			result[i / 4] += ((M4x4)a)[i / 4][i % 4] * ((vect4)b)[i % 4];
			i++;
		}
		return result;
	}


	M4x4 operator*(const M4x4& a, const M4x4& b)
	{
		M4x4 result;

		int i = 0, j;
		while (i < 16)
		{
			result[i / 4][i % 4] = 0;
			j = 0;
			while (j < 4)
			{
				result[i / 4][i % 4] += ((M4x4)a)[i / 4][j] * ((M4x4)b)[j][i % 4];
				j++;
			}
			i++;
		}
		return result;
	}

	M4x4 operator*(const M4x4& a, const float& b)
	{
		M4x4 result;

		int i = 0, j;
		while (i < 4)
		{
			j = 0;
			while (j < 4)
			{
				result[i][j] = ((M4x4)a)[i][j] * b;
				j++;
			}
			i++;
		}
		return result;
	}

	M4x4 operator+(const M4x4& a, const M4x4& b) 
	{
		M4x4 result;

		int i = 0, j;
		while (i < 4)
		{
			j = 0;
			while (j < 4)
			{
				result[i][j] = ((M4x4)a)[i][j] + ((M4x4)b)[i][j];
				j++;
			}
			i++;
		}
		return result;
	}

	M4x4 operator-(const M4x4& a, const M4x4& b)
	{
		M4x4 result;

		int i = 0, j;
		while (i < 4)
		{
			j = 0;
			while (j < 4)
			{
				result[i][j] = ((M4x4)a)[i][j] - ((M4x4)b)[i][j];
				j++;
			}
			i++;
		}
		return result;
	}


	M4x4 operator!(const M4x4& a) 
	{
		M4x4 result;
		float o[4][4], inv[4][4], div, f1, f2, f3;
		int i = 0, j;
		while (i < 4)
		{
			j = 0;
			while (j < 4)
			{
				o[i][j] = a.m[i][j];
				inv[i][j] = 0;
				j++;
			}
			inv[i][i] = 1;
			i++;
		}
		i = 0;
		while (i < 4) 
		{
			j = 0;
			div = 1 / o[i][i];
			f1 = -o[(i + 1) % 4][i];
			f2 = -o[(i + 2) % 4][i];
			f3 = -o[(i + 3) % 4][i];
			while (j < 4)
			{
				o[i][j] *= div;
				o[(i + 1) % 4][j] += o[i][j] * f1;
				o[(i + 2) % 4][j] += o[i][j] * f2;
				o[(i + 3) % 4][j] += o[i][j] * f3;
				inv[i][j] *= div;
				inv[(i + 1) % 4][j] += inv[i][j] * f1;
				inv[(i + 2) % 4][j] += inv[i][j] * f2;
				inv[(i + 3) % 4][j] += inv[i][j] * f3;
				j++;
			}
			i++;
		}
		i = 0;
		while (i < 4)
		{
			j = 0;
			while (j < 4)
			{
				result[i][j] = (float)inv[i][j];
				j++;
			}

			i++;
		}
		return result;
	}

	bool operator==(const M4x4& a, const M4x4& b)
	{
		
		int i = 0, j;
		while (i < 4)
		{
			j = 0;
			while (j < 4)
			{
				if (!(((M4x4)a)[i][j] == ((M4x4)b)[i][j]))
					return false;
				j++;
			}
			i++;
		}
		return true;
	}



	//Quake's fast inverse sqrt
	float q_sqrt(float x) 
	{
		long i;
		float x2, y = x;

		const float threehalfs = 1.5F;

		x2 = x * 0.5F;
		i = *(long*)&y;
		i = 0x5f3759df - (i >> 1); //What tha fuck???
		y = *(float*)&i;
		y = y * (threehalfs - (x2 * y * y));
		//y = y * (threehalfs - (x2 * y * y));
	
		return y;
	}

	vect4 operator+(const vect4& a, const vect4& b)
	{
		vect4 result;
		result.x = a.x + b.x;
		result.y = a.y + b.y;
		result.z = a.z + b.z;
		result.w = a.w + b.w;
		return result;
	}
	vect4 operator-(const vect4& a, const vect4& b)
	{
		vect4 result;
		result.x = a.x - b.x;
		result.y = a.y - b.y;
		result.z = a.z - b.z;
		result.w = a.w - b.w;
		return result;
	}
	vect4 operator*(const vect4& a, const vect4& b)
	{
		vect4 result;
		
		
		return result;
	}
	vect4 operator*(const vect4& a, const float& b)
	{
		vect4 result;
		result.x = a.x * b;
		result.y = a.y * b;
		result.z = a.z * b;
		result.w = a.w * b;
		return result;
	}
	vect4 operator!(const vect4& a)//normalization
	{
		vect4 result;
		float inv_length;
		inv_length = q_sqrt(a.x * a.x
			+ a.y * a.y
			+ a.z * a.z
			+ a.w * a.w);

		result.x = a.x * inv_length;
		result.y = a.y * inv_length;
		result.z = a.z * inv_length;
		result.w = a.w * inv_length;

		return result;
	}
	float operator^(const vect4& a, const vect4& b)//dot product
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	vect3 operator+(const vect3& a, const vect3& b)
	{
		vect3 result;
		result.x = a.x + b.x;
		result.y = a.y + b.y;
		result.z = a.z + b.z;

		return result;
	}
	vect3 operator-(const vect3& a, const vect3& b)
	{
		vect3 result;
		result.x = a.x - b.x;
		result.y = a.y - b.y;
		result.z = a.z - b.z;

		return result;
	}
	vect3 operator*(const vect3& a, const vect3& b)
	{
		vect3 result;
		result.x = a.y * b.z - b.y * a.z;
		result.y = a.z * b.x - b.z * a.x;
		result.z = a.x * b.y - b.x * a.y;
		return result;
	}
	vect3 operator*(const vect3& a, const float& b) 
	{
		vect3 result;
		result.x = a.x * b;
		result.y = a.y * b;
		result.z = a.z * b;
		return result;
	}
	vect3 operator!(const vect3& a) 
	{
		vect3 result;
		float inv_length;
		inv_length = q_sqrt(a.x* a.x 
		+ a.y * a.y
		+ a.z * a.z);
		
		result.x = a.x * inv_length;
		result.y = a.y * inv_length;
		result.z = a.z * inv_length;
		
		return result;
	}
	float operator^(const vect3& a, const vect3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	vect2 operator+(const vect2& a, const vect2& b)
	{
		vect2 result;
		result.x = a.x + b.x;
		result.y = a.y + b.y;		

		return result;
	}
	vect2 operator-(const vect2& a, const vect2& b)
	{
		vect2 result;
		result.x = a.x - b.x;
		result.y = a.y - b.y;

		return result;
	}
	float operator*(const vect2& a, const vect2& b) 
	{
		return a.x * b.y - a.y * b.x;
	}
	vect2 operator*(const vect2& a, const float& b)
	{
		vect2 result;
		result.x = a.x * b;
		result.y = a.y * b;
		return result;
	}
	vect2 operator!(const vect2& a)
	{
		vect2 result;
		float inv_length;
		inv_length = q_sqrt(a.x * a.x
			+ a.y * a.y);

		result.x = a.x * inv_length;
		result.y = a.y * inv_length;

		return result;
	}
	float operator^(const vect2& a, const vect2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	float Dist(vect3 a, vect3 b)
	{
		vect3 c;
		c = a - b;
		return 1/q_sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
	}
}