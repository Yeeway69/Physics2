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
	// Example: Initialize a player body (this is just for demonstration and can be adapted as needed)
	Body* player = new Body();
	player->position = { 100.0f, 100.0f }; // starting position
	player->velocity = { 0.0f, 0.0f }; // starting velocity
	player->acceleration = gravity; // affected by gravity
	player->mass = 1.0f; // arbitrary mass
	player->elasticity = 0.5f; // some bounce
	player->friction = 0.1f; // some friction
	bodies.push_back(player);


	//Creating other bodies
	
	star->position = { 300.0f, 100.0f };
	star->acceleration = { 0.0f, -9.81f };
	star->mass = 100000.0f;
	bodies.push_back(star);
	
	planet1->position = { 400.0f, 100.0f };
	planet1->acceleration = { 0.0f, -9.81f };
	planet1->mass = 1000.0f;
	bodies.push_back(planet1);
	
	planet2->position = { 200.0f, 100.0f };
	planet2->acceleration = { 0.0f, -9.81f };
	planet2->mass = 100.0f;
	bodies.push_back(planet2);


	//Creating other objects
	object1->position = { 500.0f, 100.0f }; 
	object1->velocity = { 10.0f, 0.0f }; 
	object1->acceleration = { 0.0f, 0.0f };
	object1->mass = 1.0f; 
	object1->friction = 0.1f; 
	bodies.push_back(object1);

	object2->position = { 600.0f, 100.0f }; 
	object2->velocity = { 20.0f, 0.0f }; 
	object2->acceleration = { 0.0f, 0.0f };
	object2->mass = 2.0f; 
	object2->friction = 0.1f; 
	bodies.push_back(object2);

	// Example: Initialize a platform body (this is just for demonstration and can be adapted as needed)
	int width, height;
	App->window->GetWindowSize(width, height);
	groundHeight = height / 2.0f; // Initialize ground halfway up the window

	//firing
	cannonAngle = 45.0f;  // Default to 45 degrees
	cannonPower = 60.0f;   // Default power
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




	// Distances between the bodies
	fPoint dist1 = { star->position.x - planet1->position.x, star->position.y - planet1->position.y };
	fPoint dist2 = { star->position.x - planet2->position.x, star->position.y - planet2->position.y };
	fPoint dist3 = { object1->position.x - object2->position.x, object1->position.y - object2->position.y };

	// Magnitude of the distances
	float r1 = sqrt(dist1.x * dist1.x + dist1.y * dist1.y);
	float r2 = sqrt(dist2.x * dist2.x + dist2.y * dist2.y);
	float r3 = sqrt(dist3.x * dist3.x + dist3.y * dist3.y);

	// Calculating the gravitational forces Newton Law
	fPoint force1 = NGUL(star->mass, planet1->mass, r1);
	fPoint force2 = NGUL(star->mass, planet2->mass, r2);
	fPoint force3 = NGUL(object1->mass, object2->mass, r3);

	// Updating accelerations of planet1 and planet2 based on the forces
	planet1->acceleration.x = force1.x / planet1->mass;
	planet1->acceleration.y = force1.y / planet1->mass;
	planet2->acceleration.x = force2.x / planet2->mass;
	planet2->acceleration.y = force2.y / planet2->mass;
	object1->acceleration.x = force3.x / object1->mass;
	object1->acceleration.y = force3.y / object1->mass;
	object2->acceleration.x = force3.x / object2->mass;
	object2->acceleration.y = force3.y / object2->mass;
	


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

	//float deltaTime = 0.016f; // Assuming 60 FPS for now

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
