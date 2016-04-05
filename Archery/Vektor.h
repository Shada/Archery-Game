#pragma once
#include <math.h>

class Vektor
{
public:
	Vektor();
	Vektor(float x, float y, float z);
	float Length();
	float x, y, z;
	void SetVektor(float x, float y, float z);
	void Stop();
	~Vektor();
};