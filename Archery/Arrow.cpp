#include "Arrow.h"

extern hgeSprite *hole;

Arrow::Arrow(string name, float mass, float c1, float c2)
{
	aWeight = mass;
	Arrow::name = name;
	Arrow::c1 = c1;
	Arrow::c2 = c2;
	angleUD = 0.0f;
	angleSTS = 0.0f;
	hit = false;
}

Arrow::Arrow(Arrow *arrow)
{
	aWeight = arrow->GetWeight();
	name = arrow->GetName();
	c1 = arrow->GetC1();
	c2 = arrow->GetC2();
	angleUD = 0.0f;
	angleSTS = 0.0f;
	hit = false;
}

void Arrow::Render(hgeSprite* sprite)
{
	sprite->RenderEx(position.y, position.z, angleUD, scaleFactor, 0.0F);
}

void Arrow::Update(float dt)
{
	Physics::Forces(velocity, c1, aWeight, dt);
	Physics::PositionChange(dt, position, velocity);
	Physics::ArrowAngle(velocity, angleUD, angleSTS);
}

bool Arrow::Search(string item)
{
	if(item == name)
		return true;
	return false;
}

bool Arrow::HitGround()
{
	if(position.z + scaleFactor * 158 * cos(M_PI / 2 - angleUD) > 600)
		return true;
	return false;
}

bool Arrow::ReachTarget()
{
	if(position.y + 158 * scaleFactor * sin(M_PI / 2 - angleUD) * sin(M_PI / 2 - angleSTS) > 1080)
		return true;
	return false;
}

bool Arrow::HitTarget()
{
	float passedy = position.y + 158 * scaleFactor * sin(M_PI / 2 - angleUD) * sin(M_PI / 2 - angleSTS);
	//1. find distance to target
	float distance = passedy - 1080;
	//2. find time
	float deltaTime = distance / velocity.y;
	//3. go back
	xhit = position.x - velocity.x * deltaTime;
	zhit = position.z - velocity.z * deltaTime;
	zhit = zhit + 158 * sin(angleUD) * scaleFactor;
	zhit -= 600 - 444 * scaleFactor + 120 * scaleFactor;
	float yhit = 1080;

	if((xhit < 120 * scaleFactor && xhit > -(120 * scaleFactor)) && (zhit < 120 * scaleFactor && zhit > -(120 * scaleFactor)))
	{
		xhit *= (2.5 / scaleFactor);
		zhit *= (2.5 / scaleFactor);
		for(int c = 1; c < 6; c++)
		{
			if(sqrt(xhit * xhit + zhit * zhit) < 60 * c + 0.2 * (hole->GetHeight() / 2))
			{
				hit = true;
				points += (6 - c);
				break;
			}
		}
		
		return true;
	}
	return false;
}
string Arrow::GetName()
{
	return name;
}
float Arrow::GetWeight()
{
	return aWeight;
}
float Arrow::GetC1()
{
	return c1;
}
float Arrow::GetC2()
{
	return c2;
}

float Arrow::GetAngle()
{
	return angleUD;
}
Vektor Arrow::GetPosition()
{
	return position;
}
Vektor Arrow::GetVelocity()
{
	return velocity;
}

void Arrow::SetVelocity(float x, float y, float z)
{
	Arrow::velocity.SetVektor(x, y, z);
}

void Arrow::SetVelocity(Vektor velocity)
{
	Arrow::velocity = Vektor(velocity.x, velocity.y, velocity.z);
}
void Arrow::SetAngle(float angle)
{
	Arrow::angleUD = angle;
}
void Arrow::SetPosition(float x, float y, float z)
{
	Arrow::position.SetVektor(x, y, z);
}


void Arrow::Stop()
{
	velocity.Stop();
}
Arrow::~Arrow()
{
}
