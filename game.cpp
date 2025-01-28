// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#include "precomp.h"
#include "game.h"
#include <ctime> 
#include "imgui.h"

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	//seed the random number generator
	time_t time_ptr;
	time_ptr = time(NULL);
	tm* tm_local = localtime(&time_ptr);
	InitSeed(static_cast<uint>(tm_local->tm_sec * tm_local->tm_min * tm_local->tm_hour * tm_local->tm_wday * tm_local->tm_year));


	for (int i = 0; i < 1000; i++) {
		boids.push_back(new Boid(screen, &boids));
	}
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void Game::Tick( float  deltaTime  )
{
	deltaTime = min( deltaTime, 0.1f );
	screen->Clear( 0 );
#ifdef _DEBUG
	Boid* mBoid = boids.at(currentBoid);
	mBoid->mColor = 0xff0000;
	ImGui::Begin("Boid Settings");
	ImGui::SliderFloat("Boid Length", &mBoid->BOIDLENGTH, 0, 100);
	ImGui::SliderFloat("Local Radius", &mBoid->LOCALRADIUS, 0, 100);
	ImGui::SliderFloat("Max Force", &mBoid->MAXFORCE, 0, 2);
	ImGui::SliderInt("Boid", &currentBoid, 0, boids.size() - 1);

	ImGui::Text("Boid %d", currentBoid);
	ImGui::Text("Position: (%f, %f)", mBoid->mPosition.x, mBoid->mPosition.y);
	ImGui::Text("Velocity: (%f, %f)", mBoid->mVelocity.x, mBoid->mVelocity.y);
	ImGui::Text("Acceleration: (%f, %f)", mBoid->mAcceleration.x, mBoid->mAcceleration.y);
	ImGui::Text("Acceleration Length: %f", length(mBoid->mAcceleration));
	ImGui::Checkbox("Show Desired Cohesion & Allignment", &showDesiredCohesionAllignment);
	if (ImGui::Checkbox("Allignment", &allignment) || ImGui::Checkbox("Cohesion", &cohesion) || ImGui::Checkbox("Separation", &separation)) {
		for (int i = 0; i < boids.size(); i++) {
			boids[i]->allignment = allignment;
			boids[i]->cohesion = cohesion;
			boids[i]->seperation = separation;
		}
	}


	ImGui::End();
	if(showDesiredCohesionAllignment)mBoid->DebugDraw();
#endif

	for (int i = 0; i < boids.size(); i++) {
		boids[i]->Update(deltaTime);
		boids[i]->Draw();
	}
}