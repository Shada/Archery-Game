#include "Vektor.h"


Vektor::Vektor()
{
	x = 0;
	y = 0;
	z = 0;
}
Vektor::Vektor(float x, float y, float z)
{
	Vektor::x = x;
	Vektor::y = y;
	Vektor::z = z;
}
void Vektor::SetVektor(float x, float y, float z)
{
	Vektor::x = x;
	Vektor::y = y;
	Vektor::z = z;
}

float Vektor::Length()
{
	return sqrt(x * x + y * y + z * z);
}

void Vektor::Stop()
{
	x = 0;
	y = 0;
	z = 0;
}
Vektor::~Vektor()
{

}