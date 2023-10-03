#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
//include SDL2 libraries
#include "SDL/include/SDL.h"
using namespace std;
//include module renderer
#include "ModuleRender.h"
#include "ModuleInput.h"
#include <string>
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
//include <cmath> for trigonometric functions
#include <cmath>
#define DEGTORAD(angleDegrees) ((angleDegrees) * M_PI / 180.0)




ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
	// Example: Initialize a player body (this is just for demonstration and can be adapted as needed)
	Body* player = new Body();
	player->position = { 100.0f, 100.0f }; // starting position
	player->velocity = { 0.0f, 0.0f }; // starting velocity
	player->acceleration = gravity; // affected by gravity
	player->mass = 1.0f; // arbitrary mass
	player->elasticity = 0.5f; // some bounce
	player->friction = 0.1f; // some friction
	bodies.push_back(player);

	// Example: Initialize a platform body (this is just for demonstration and can be adapted as needed)
	int width, height;
	App->window->GetWindowSize(width, height);
	groundHeight = height / 2.0f; // Initialize ground halfway up the window

	//firing
	cannonAngle = 45.0f;  // Default to 45 degrees
	cannonPower = 5.0f;   // Default power
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
	return true;
}

void ModulePhysics::ApplyForce(Body& body, const fPoint& force)
{
	fPoint acceleration = { force.x / body.mass, force.y / body.mass };
	body.velocity += acceleration;
}

void ModulePhysics::UpdateWindowTitle()
{
    std::string scheme;
    switch (currentScheme)
    {
        case EULER: scheme = "Euler"; break;
        case SYMPLECTIC_EULER: scheme = "Symplectic Euler"; break;
        case VELOCITY_VERLET: scheme = "Velocity-Verlet"; break;
    }

    std::string title = "Scheme: " + scheme + " | Angle: " + std::to_string(cannonAngle) + "° | Power: " + std::to_string(cannonPower);
    SDL_SetWindowTitle(App->window->window, title.c_str());
}


update_status ModulePhysics::PreUpdate()
{

	// Hotkey to adjuct cannon ball shooting direction
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		cannonAngle -= 1.0f;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		cannonAngle += 1.0f;
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)  // Adjust as needed based on your input handling
	{
		cannonPower += 0.1f;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)  // Adjust as needed based on your input handling
	{
		cannonPower -= 0.1f;
	}


	// Cannon Representation
	int width, height;
	App->window->GetWindowSize(width, height);
	const fPoint cannonPosition = { 150, (float)height - 350 };


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) // Listen for SPACEBAR press
	{
		Body* cannonball = new Body();
		cannonball->position = cannonPosition;
		cannonball->velocity = { -2.0f, -4.0f }; // Initial velocity (adjust as needed)
		cannonball->acceleration = gravity; // Affected by gravity
		cannonball->mass = 1.0f; // Arbitrary mass
		cannonball->elasticity = 0.5f; // Some bounce
		cannonball->friction = 0.1f; // Some friction
		bodies.push_back(cannonball);

		float radianAngle = DEGTORAD(cannonAngle);  // Convert angle  to radians
		cannonball->velocity.x = cannonPower * cosf(radianAngle);
		cannonball->velocity.y = -cannonPower * sinf(radianAngle);  // Negative because y is up

	}

	float deltaTime = 0.016f; // Assuming 60 FPS for now

	// Iterate over each physics body and update
	for (std::list<Body*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
	{
		Body* body = *it;
		fPoint halfTimeVelocity;

		// Switch integration schemes based on currentScheme
		switch (currentScheme)
		{
		case EULER:
			body->position.x += body->velocity.x * deltaTime;
			body->position.y += body->velocity.y * deltaTime;
			body->velocity.x += body->acceleration.x * deltaTime;
			body->velocity.y += body->acceleration.y * deltaTime;
			break;

		case SYMPLECTIC_EULER:
			body->velocity.x += body->acceleration.x * deltaTime;
			body->velocity.y += body->acceleration.y * deltaTime;
			body->position.x += body->velocity.x * deltaTime;
			body->position.y += body->velocity.y * deltaTime;
			break;

		case VELOCITY_VERLET:
			halfTimeVelocity.x = body->velocity.x + (0.5f * body->acceleration.x * deltaTime);
			halfTimeVelocity.y = body->velocity.y + (0.5f * body->acceleration.y * deltaTime);
			body->position.x += halfTimeVelocity.x * deltaTime;
			body->position.y += halfTimeVelocity.y * deltaTime;
			// Assuming new acceleration is computed based on forces later in the loop
			body->velocity.x = halfTimeVelocity.x + (0.5f * body->acceleration.x * deltaTime);
			body->velocity.y = halfTimeVelocity.y + (0.5f * body->acceleration.y * deltaTime);
			break;
		}

		body->velocity += gravity * deltaTime;

		// Check for collisions with the ground
		if (body->position.y >= height - 300) // Assuming 10 is the radius of the cannonball
		{
			body->velocity.y = -body->elasticity * body->velocity.y; // Reflect and reduce vertical velocity
			body->position.y = height - 300; // Reset position to be just on the ground
		}
		//Friction
		if (body->position.y >= height - 10)
		{
			body->velocity.x *= (1 - body->friction); // Apply friction to horizontal velocity
		}
		//Limiting Bounces:
		if (abs(body->velocity.y) < 0.1f)
		{
			body->velocity.y = 0.0f;
		}

	}

	


	return UPDATE_CONTINUE;
}

update_status ModulePhysics::PostUpdate()
{

	//draw the ground with a line
	int width, height;
	App->window->GetWindowSize(width, height);
	App->renderer->DrawLine(0, height - 290, width, height - 300, 255, 255, 255);



	// In the PostUpdate method:
	for (list<Body*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
	{
		Body* body = *it;
		App->renderer->DrawCircle(body->position.x, body->position.y, 10, 255, 0, 0); // Drawing the cannonball
	}


	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	// Hotkey detection to switch integration schemes
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		currentScheme = static_cast<IntegrationScheme>((currentScheme + 1) % 3);
	}

	//title display
	//the scheme switch
	UpdateWindowTitle();

	


	if (debug)
	{
		for (list<Body*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
		{
			Body* body = *it;
			App->renderer->DrawCircle(body->position.x, body->position.y, 10, 255, 0, 0);
		}
	}

	return UPDATE_CONTINUE;
}




// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");
	return true;
}
