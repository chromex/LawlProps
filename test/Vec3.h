#ifndef VEC3_H
#define VEC3_H

struct vec3
{
	vec3()
	{
		x = y = z = 0.f;
	}

	vec3(float v)
	{
		x = y = z = v;
	}

	float x, y, z;
};

#endif