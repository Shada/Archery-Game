#pragma once
#include "Vektor.h"
#include "Bow.h"
#include "stdafx.h"

class Physics
{

public:
	
	
	static Vektor ArrowInitialVelocity(Bow *bow, float aWeight, float angleUD, float angleSTS);

	static void Forces(Vektor &velocity, float c, float aWeight, float dt);

	static Vektor Gravity(float aWeight);

	static Vektor AirResistance(Vektor &velocity, float c);

	static void VelocityChange(Vektor &velocity, Vektor acc, float dt);

	static void PositionChange(float dt, Vektor &position, Vektor velocity);
	
	static void ArrowAngle(Vektor velocity, float &angleUS, float &angeSTS);
	
	static Vektor CrossProduct(Vektor first, Vektor second);
	
	static float DotProduct(Vektor vec1, Vektor vec2);

	static float DegreeToRad(float angle);

	static float WindAngle(Vektor wind);

	static Vektor Wind();

	static float Round(float x);
};

