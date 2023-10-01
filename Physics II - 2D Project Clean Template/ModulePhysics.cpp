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

update_status ModulePhysics::PreUpdate()
{
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
	}

	return UPDATE_CONTINUE;
}

update_status ModulePhysics::PostUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	// Hotkey detection to switch integration schemes
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		currentScheme = static_cast<IntegrationScheme>((currentScheme + 1) % 3);
	}

	const char* schemeNames[] = { "EULER", "SYMPLECTIC_EULER", "VELOCITY_VERLET" };
	std::string title = "Physics Simulation - Current Scheme: ";
	title += schemeNames[currentScheme];
	App->window->SetTitle(title.c_str());

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
