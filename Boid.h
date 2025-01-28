#pragma once
class Boid
{
public:
	Boid(Surface* screen, vector<Boid*>* boids);
	void Update(float deltaTime);
	void Draw();
	void DebugDraw();
	//settings:
	float BOIDLENGTH = 10.0f;
	float LOCALRADIUS = 50.0f;
	float MAXFORCE = 0.5f;
	float MAXVEL = 3.0f;

	uint mColor = 0xffffff;

	//data
	float2 mPosition;
	float2 mVelocity;
	float2 mAcceleration;
	bool allignment = true;
	bool cohesion = true;
	bool seperation = true;
private:
	void Edges();
	void Align();


	float2 averageVel;
	float2 averagePos;
	float2 averageDiff;


	vector<Boid*>* mpBoids;
	Surface* mpScreen;

	

	//my own
	float distanceFloat2(float2 a, float2 b)
	{
		return sqrt(pow(b.x - a.x, 2.0f) + pow(b.y - a.y, 2.0f));
	}

};

