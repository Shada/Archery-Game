#pragma once
#include "Vektor.h"
#include "Physics.h"
#include "stdafx.h"
#include <string>
using namespace std;

class Arrow
{
private:

	string name;
	float aWeight, c1, c2, angleUD, angleSTS;
	Vektor position;
	Vektor velocity;
	
public:
	Arrow(string name, float mass, float c1, float c2);
	Arrow(Arrow *arrow);
	
	float xhit, zhit;
	bool hit;
	void Update(float dt);
	void Render(hgeSprite* sprite);
	bool Search(string item);
	bool HitGround();
	bool ReachTarget();
	bool HitTarget();

	string GetName();
	float GetWeight();
	float GetC1();
	float GetC2();
	float GetAngle();
	Vektor GetPosition();
	Vektor GetVelocity();

	void SetVelocity(float x, float y, float z);
	void SetVelocity(Vektor velocity);
	void SetAngle(float angle);
	void SetPosition(float x, float y, float z);

	void Stop();


	~Arrow();
};

