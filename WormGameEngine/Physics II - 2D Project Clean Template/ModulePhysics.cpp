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
#include <algorithm>





ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
	// Example: Initialize a platform body (this is just for demonstration and can be adapted as needed)
	int width, height;
	App->window->GetWindowSize(width, height);
	groundHeight = height / 2.0f; // Initialize ground halfway up the window
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
    // Scheme
    std::string scheme;
    switch (currentScheme)
    {
        case EULER: scheme = "Euler"; break;
        case SYMPLECTIC_EULER: scheme = "Symplectic Euler"; break;
        case VELOCITY_VERLET: scheme = "Velocity-Verlet"; break;
    }

    // Framerate Mode
    std::string framerateModeStr;
    switch (currentMode)
    {
        case FRM_FIXED: framerateModeStr = "Fixed Framerate"; break;
        case FRM_VARIABLE: framerateModeStr = "Variable Framerate"; break;
        case FRM_SEMI_FIXED: framerateModeStr = "Semi-Fixed Framerate"; break;
    }

    std::string fpsInfo = " | Target FPS: " + std::to_string(targetFPS) + 
                          " | Current FPS: " + std::to_string(currentFPS);
   
    // Consolidate all information into a single title string
    std::string title = "Framerate Mode: " + framerateModeStr + fpsInfo + 
                        " | Scheme: " + scheme + 
                        " | Angle: " + std::to_string(cannonAngle) + "° | Power: " + std::to_string(cannonPower);
    
    SDL_SetWindowTitle(App->window->window, title.c_str());
}



update_status ModulePhysics::PreUpdate()
{
	//Calculate Frame Time:
	currentFrameTime = SDL_GetTicks() / 1000.0f; // Convert milliseconds to seconds
	float deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;  // Update lastFrameTime for the next frame

	const float maxDeltaTime = 1.0f / 10.0f; // Maximum allowed deltaTime
    #undef min
	deltaTime = std::min(deltaTime, maxDeltaTime);


	//Implement Framerate Control Logic:
	switch (currentMode)
	{
		case FRM_FIXED:
		{
			float frameDelay = targetFrameTime * 1000.0f - deltaTime * 1000.0f; // Convert to milliseconds
			if (frameDelay > 0)
				SDL_Delay((Uint32)frameDelay);
		
			// Update debug stats
			currentFPS = 1.0f / targetFrameTime;
			currentFrameTime = targetFrameTime;
			break;
		}
	
		case FRM_VARIABLE:
		{
			// No artificial delay, simply compute the debug stats
			currentFPS = 1.0f / deltaTime;
			currentFrameTime = deltaTime;
			break;
		}
	
		case FRM_SEMI_FIXED:
		{
			// Physics and logic calculations at a fixed rate, rendering as fast as possible
			if (deltaTime < fixedFrameTime)
			{
				// If not enough time has passed for the next fixed update, delay
				float frameDelay = fixedFrameTime * 1000.0f - deltaTime * 1000.0f; // Convert to milliseconds
				SDL_Delay((Uint32)frameDelay);
			}

			// For simplicity, in this example, we're only doing the fixed update.
			// In a real-world scenario, you'd run the rendering logic as fast as possible here.

			// Update debug stats
			currentFPS = 1.0f / fixedFrameTime;
			currentFrameTime = fixedFrameTime;
			break;
		}
	}


	// Cannon Representation
	int width, height;
	App->window->GetWindowSize(width, height);

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

	
	lastFrameTime = currentFrameTime;

	return UPDATE_CONTINUE;
}

update_status ModulePhysics::PostUpdate()
{

	//draw the ground with a line
	int width, height;
	App->window->GetWindowSize(width, height);
	App->renderer->DrawLine(0, height - 290, width, height - 300, 255, 255, 255);
	
	


	// In the PostUpdate method


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

	//HotKeys to Swap Framerate control Schemes
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		currentMode = FRM_FIXED;
		UpdateWindowTitle();
	}
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		currentMode = FRM_VARIABLE;
		UpdateWindowTitle();
	}
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		currentMode = FRM_SEMI_FIXED;
		UpdateWindowTitle();
	}

	//Handle Input for FPS Control:
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		targetFPS += 10;
		targetFrameTime = 1.0f / targetFPS;
		UpdateWindowTitle();
	}
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		targetFPS -= 10;
		if (targetFPS < 1) targetFPS = 1;  // Ensure it doesn't go below 1
		targetFrameTime = 1.0f / targetFPS;
		UpdateWindowTitle();
	}

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
