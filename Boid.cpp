#include "precomp.h"
#include "Boid.h"
#include "tmpl8math.h"

Boid::Boid(Surface* screen, vector<Boid*>* boids) {
	this->mpScreen = screen;
	this->mpBoids = boids;
	mPosition = float2(SCRWIDTH * RandomFloat(), SCRHEIGHT * RandomFloat());
	mVelocity = float2(1, 1);
	mAcceleration = float2(0, 0);

	//randomise velocity
	mVelocity *= float2(RandomFloat()*2-1, RandomFloat()*2-1);

}
void Boid::Update(float deltaTime) {
	//update the boid

	//wrap around the screen
	Edges();
	//align to the average velocity of the boids around this boid
	Align();
	//move the boid
	mPosition += mVelocity * deltaTime;
	mVelocity += mAcceleration * deltaTime;
	mVelocity = length(mVelocity) < MAXVEL ? mVelocity : normalize(mVelocity) * MAXVEL;

}

void Boid::Edges() {
	//wrap around the screen
	if (mPosition.x > SCRWIDTH) {
		mPosition.x = 0;
	}
	if (mPosition.x < 0) {
		mPosition.x = SCRWIDTH;
	}
	if (mPosition.y > SCRHEIGHT) {
		mPosition.y = 0;
	}
	if (mPosition.y < 0) {
		mPosition.y = SCRHEIGHT;
	}
}

void Boid::Align()
{
	//get the average velocity of the boids around this boid
	//set the acceleration to be in the direction of the average velocity
	averageVel = {0.0f,0.0f};
	averagePos = {0.0f,0.0f};
	averageDiff = { 0.0f,0.0f };

	int count = 0;
	for (int i = 0; i < mpBoids->size(); i++) {
		//dont include this boid in the average
		if (mpBoids->at(i) != this) {
			float distance = distanceFloat2(mPosition, mpBoids->at(i)->mPosition);
			if (distance < LOCALRADIUS) {
				averageVel += mpBoids->at(i)->mVelocity;
				averagePos += mpBoids->at(i)->mPosition;
				count++;
				float2 differenceVector = mPosition - mpBoids->at(i)->mPosition;
				differenceVector /= distance;
				averageDiff += differenceVector;

			}
		}
	}
	if (count == 0) {
		//nothing to allign to
		return;
	}
	averageDiff /= count;
	averageVel /= count; //average velocity
	averagePos /= count; //average position

	//steer towards the average velocity
	float2 Avel = averageVel - this->mVelocity;
	//steer towards the average position
	float2 Apos = averagePos - this->mPosition;

	////apply a max force
	Apos = normalize(Apos) * MAXFORCE;

	Avel = normalize(Avel) * MAXFORCE;

	averageDiff = normalize(averageDiff) * MAXFORCE;

	if(isnan(averageDiff.x)|| isnan(averageDiff.y)) averageDiff = {0.0f,0.0f};
	if (isnan(Apos.x) || isnan(Apos.y)) Apos = { 0.0f,0.0f };
	if (isnan(Avel.x) || isnan(Avel.y)) Avel = { 0.0f,0.0f };


	//limit velocity
	Avel = length(Avel) < MAXFORCE ? Avel : normalize(Avel) * MAXFORCE;
	Apos = length(Apos) < MAXFORCE ? Apos : normalize(Apos) * MAXFORCE;
	averageDiff = length(averageDiff) < MAXFORCE ? averageDiff : normalize(averageDiff) * MAXFORCE;
	mAcceleration = 0;
	//mAcceleration = acceleration;
	if(allignment == true) mAcceleration += Avel;
	if(cohesion == true) mAcceleration += Apos;
	if(seperation == true) mAcceleration += averageDiff;
}

void Boid::Draw() {

	float2 velDirection = normalize(mVelocity);
	mpScreen->Box(mPosition.x -2, mPosition.y -2 , mPosition.x + 2, mPosition.y + 2, mColor);
	mpScreen->Line(mPosition.x, mPosition.y, mPosition.x + (velDirection.x * BOIDLENGTH), mPosition.y + (velDirection.y * BOIDLENGTH), mColor);
}

void Boid::DebugDraw() {
	float2 velDirection = { 0.0f,0.0f };
	if(averageVel != float2(0.0f,0.0f) ) velDirection = normalize(averageVel);
	mpScreen->Box(averagePos.x - 2, averagePos.y - 2, averagePos.x + 2, averagePos.y + 2, 0x00ff00);
	mpScreen->Line(averagePos.x, averagePos.y, averagePos.x + (velDirection.x * BOIDLENGTH), averagePos.y + (velDirection.y * BOIDLENGTH), 0x00ff00);
}
