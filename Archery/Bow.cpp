#include "Bow.h"

Bow::Bow(string name, float mass, float Ep, float k)
{
	
	Bow::Ep = Ep;
	Bow::name = name;
	Bow::bWeight = mass;
	Bow::k=k;

}

bool Bow::Search(string item)
{
	if(item == name)
		return true;
	return false;
}


float Bow::GetWeight()
{
	return bWeight;
}
float Bow::GetK()
{
	return k;
}
string Bow::GetName()
{
	return name;
}
float Bow::GetEp()
{
	return Ep;
}

Bow::~Bow()
{
}