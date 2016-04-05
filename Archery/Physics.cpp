#include "Physics.h"

Vektor Physics::ArrowInitialVelocity(Bow *bow, float aWeight, float angleUD, float angleSTS)
{
	// Calculate initiate speed for the arrow.
	float velocity = (sqrt(bow->GetEp() * 2 / (aWeight + bow->GetK() * bow->GetWeight())) * 100);


	// Convert angle from degrees to radians.
	float angleUDrad = M_PI / 2 - Physics::DegreeToRad(angleUD);
	float angleSTSrad = M_PI / 2 - Physics::DegreeToRad(angleSTS);


	// Using Spherical coordinate system to find out the velocity-vector using 2 angles.
	float v_x = velocity * sin(angleUDrad) * cos(angleSTSrad);
	float v_y = velocity * sin(angleUDrad) * sin(angleSTSrad);
	float v_z = velocity * cos(angleUDrad);

	return Vektor(v_x, v_y, v_z);
}

void Physics::Forces(Vektor &velocity, float c, float aWeight, float dt)
{
	Vektor Fg = Physics::Gravity(aWeight);

	Vektor Fd = Physics::AirResistance(velocity, c);
	if(velocity.z < 0)
		Fd.z = -Fd.z;
	if(velocity.x < 0)
		Fd.x = -Fd.x;
	Vektor Ft = Vektor(Fd.x + Fg.x, Fd.y + Fg.y, Fd.z + Fg.z);

	Vektor acc = Vektor((Ft.x / aWeight), (Ft.y / aWeight), (Ft.z / aWeight));

	Physics::VelocityChange(velocity, acc, dt);


}

Vektor Physics::Gravity(float aWeight)
{
	float Fz = GRAV * aWeight;

	return Vektor(0, 0, Fz); 
}

Vektor Physics::AirResistance(Vektor &velocity, float c)
{
	//F=c*v^2

	float Fx = c * pow(velocity.x, 2);
	float Fy = c * pow(velocity.y, 2);
	float Fz = c * pow(velocity.z, 2);

	return Vektor(Fx, Fy, Fz);
}

void Physics::VelocityChange(Vektor &velocity, Vektor acc, float dt)
{

	velocity.SetVektor(velocity.x - acc.x * dt, velocity.y - acc.y * dt, velocity.z - acc.z * dt);

}

void Physics::PositionChange(float dt, Vektor &position, Vektor velocity)
{
	position.x += velocity.x * dt * (2 * scaleFactor); 
	position.y += velocity.y * dt * (2 * scaleFactor); 
	position.z -= velocity.z * dt * (2 * scaleFactor);
}

void Physics::ArrowAngle(Vektor velocity, float &angleUD, float &angleSTS)
{
	angleUD = -atan(velocity.z / velocity.y);
	angleSTS = atan(velocity.x / velocity.y);
}

Vektor Physics::CrossProduct(Vektor first, Vektor second)
{
	
	float x,y,z;
	
	//Räknar ut vektorprodukt mellan två vektorer
	x = first.y * second.z - first.z * second.y;
	y = first.z * second.x - first.x * second.z;
	z = first.x * second.y - first.y * second.x;

	return Vektor(x, y, z);
}

float Physics::DotProduct(Vektor vec1, Vektor vec2)
{
	float value = 0.0f;

	//Räknar ut skalärprodukt mellan två vektorer
	value += vec1.x * vec2.x;
	value += vec1.y * vec2.y;
	value += vec1.z * vec2.z;

	return value;
}

float Physics::DegreeToRad(float angle)
{
	return angle * M_PI / 180;
}

float Physics::WindAngle(Vektor wind)
{
	float angle = acos(wind.x / wind.Length());

	if(wind.y < 0 && wind.x <= 0)
		angle += abs(M_PI - angle) * 2;
		
	else if(wind.y < 0 && wind.x > 0)
		angle = 2 * M_PI - angle;
	
	return 2 * M_PI - angle;
}

Vektor Physics::Wind()
{
	float wind_x = 0; float wind_y = 0;
	
	wind_x = hge->Random_Float(-7.5f, 7.5f); // by removing half of what the maximum value can be,
	wind_y = hge->Random_Float(-7.5f, 7.5f); // roughly 50% of the values will be negative

	return Vektor(wind_x, wind_y, 0);  // wind speed will be  -7.5 < (x,y) < 7.5   => max speed = sqrt(2*7.49) ~= 10.6 m/s
}

float Physics::Round(float x)
{
	return ((x * 100 - floor(x * 100) >= 0.5) ? ceil(x * 100) : floor(x * 100)) / 100;
}


