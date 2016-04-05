#pragma once

#include <string>
using namespace std;

class Bow
{
private:
	
	string name;
	float bWeight, k, Ep;

public:

	
	Bow(string name, float mass, float Ep, float k);

	bool Search(string item);
	float GetEp();
	float GetWeight();
	float GetK();
	string GetName();

	~Bow();

	
};