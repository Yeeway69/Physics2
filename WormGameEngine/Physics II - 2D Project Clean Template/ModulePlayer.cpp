#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "ModuleInput.h"
#include <cmath>
#define DEGTORAD(angleDegrees) ((angleDegrees) * M_PI / 180.0)

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	this->app = app;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	// Example: Initialize a player body (this is just for demonstration and can be adapted as needed)
	player->position = { 200.0f, SCREEN_HEIGHT - 300 }; // starting position
	player->velocity = { 0.0f, 0.0f }; // starting velocity
	player->acceleration = gravity; // affected by gravity
	player->mass = 1.0f; // arbitrary mass
	player->elasticity = 0.5f; // some bounce
	player->friction = 0.1f; // some friction
	app->physics->bodies.push_back(player);

	//firing
	cannonAngle = 45.0f;  // Default to 45 degrees
	cannonPower = 60.0f;   // Default power
	LOG("Loading player");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	// Hotkey to adjuct cannon ball shooting direction
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		cannonAngle += 1.0f;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		cannonAngle -= 1.0f;
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && cannonPower<100)  // Adjust as needed based on your input handling
	{
		cannonPower += 0.5f;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && cannonPower>0)  // Adjust as needed based on your input handling
	{
		cannonPower -= 0.5f;
	}

	const fPoint cannonPosition = { player->position.x, player->position.y - 10 };


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN &&!hasShot) // Listen for SPACEBAR press
	{
		hasShot = true;
		Body* cannonball = new Body();
		cannonball->position = cannonPosition;
		cannonball->velocity = { -2.0f, -4.0f }; // Initial velocity (adjust as needed)
		cannonball->acceleration = gravity; // Affected by gravity
		cannonball->mass = 1.0f; // Arbitrary mass
		cannonball->elasticity = 0.5f; // Some bounce
		cannonball->friction = 0.1f; // Some friction
		app->physics->bodies.push_back(cannonball);

		float radianAngle = DEGTORAD(cannonAngle);  // Convert angle  to radians
		cannonball->velocity.x = cannonPower * cosf(radianAngle);
		cannonball->velocity.y = -cannonPower * sinf(radianAngle);  // Negative because y is up

	}

	if (hasShot) 
	{
		frameCounter++;
		if (frameCounter > 30)
		{
			hasShot = false;
			frameCounter = 0;
		}
	}
	else
	{
		float radianAngle = DEGTORAD(-cannonAngle);
		int endX = player->position.x + static_cast<int>(100 * std::cosf(radianAngle));
		int endY = player->position.y + static_cast<int>(100 * std::sinf(radianAngle));

		App->renderer->DrawLine(endX, endY, player->position.x, player->position.y, 255, 255, 255);
	}

	//This is the power var 
	App->renderer->DrawGrowingRectangle(SCREEN_WIDTH*0.05f, SCREEN_HEIGHT/2, cannonPower, 255, 255, 255,255, true);



	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{
	return UPDATE_CONTINUE;
}










