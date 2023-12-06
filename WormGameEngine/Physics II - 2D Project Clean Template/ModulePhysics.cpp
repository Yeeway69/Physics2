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
	this->app = app;
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
	//platformTexture = App->textures->Load("pinball/flipperL.png");
	towerTexture1 = App->textures->Load("Assets/Tower_1.png");
	towerTexture2 = App->textures->Load("Assets/Tower_2.png");
	towerTexture3 = App->textures->Load("Assets/Tower_3.png");
	towerTexture4 = App->textures->Load("Assets/Tower_4.png");
	towerTexture5 = App->textures->Load("Assets/Tower_5.png");
	towerTexture6 = App->textures->Load("Assets/Tower_6.png");
	towerTexture7 = App->textures->Load("Assets/Tower_7.png");
	towerTexture8 = App->textures->Load("Assets/Tower_8.png");
	ballTexture = App->textures->Load("Assets/bola.png");
	towerlive1 = 8;
	towerlive2 = 8;

	LOG("Creating Physics 2D environment");

	// Example of adding platforms
	platforms.push_back(Platform(fPoint(300, 400), 180, 70, true)); // Position (100, 300), Width 200, Height 30, is a water platform true Esta es la del agua
	//platforms.push_back(Platform(fPoint(800, 100), 150, 20, false)); // Another platform
	platforms.push_back(Platform(fPoint(500, 300), 100, 20, false)); // Another platform
	platforms.push_back(Platform(fPoint(100, 100), 100, 20, false)); // Another platform
	platforms.push_back(Platform(fPoint(900, 200), 100, 20, false)); // Another platform
	platforms.push_back(Platform(fPoint(200, 150), 100, 20, false)); // Another platform
	platforms.push_back(Platform(fPoint(700, 400), 100, 20, false)); // Another platform
	platforms.push_back(Platform(fPoint(200, 250), 100, 20, false)); // Another platform
	platforms.push_back(Platform(fPoint(600, 150), 100, 20, false)); // Another platform
	platforms.push_back(Platform(fPoint(400, 300), 100, 20, false)); // Another platform
	platforms.push_back(Platform(fPoint(200, 250), 100, 20, false)); // Another platform
	platforms.push_back(Platform(fPoint(600, 150), 100, 20, false)); // Another platform

	
	//1024 758
	//358 desde abajo
	//x: 150 y:200
	firstPlayerTower.push_back(Platform(fPoint(100, 275), 150, 25, false)); // Another platform
	firstPlayerTower.push_back(Platform(fPoint(100, 300), 150, 25, false)); // Another platform
	firstPlayerTower.push_back(Platform(fPoint(100, 325), 150, 25, false)); // Another platform
	firstPlayerTower.push_back(Platform(fPoint(100, 350), 150, 25, false)); // Another platform
	firstPlayerTower.push_back(Platform(fPoint(100, 375), 150, 25, false)); // Another platform
	firstPlayerTower.push_back(Platform(fPoint(100, 400), 150, 25, false)); // Another platform
	firstPlayerTower.push_back(Platform(fPoint(100, 425), 150, 25, false)); // Another platform
	firstPlayerTower.push_back(Platform(fPoint(100, 450), 150, 25, false)); // Another platform


	secondPlayerTower.push_back(Platform(fPoint(800, 275), 150, 25, false)); // Another platform
	secondPlayerTower.push_back(Platform(fPoint(800, 300), 150, 25, false)); // Another platform
	secondPlayerTower.push_back(Platform(fPoint(800, 325), 150, 25, false)); // Another platform
	secondPlayerTower.push_back(Platform(fPoint(800, 350), 150, 25, false)); // Another platform
	secondPlayerTower.push_back(Platform(fPoint(800, 375), 150, 25, false)); // Another platform
	secondPlayerTower.push_back(Platform(fPoint(800, 400), 150, 25, false)); // Another platform
	secondPlayerTower.push_back(Platform(fPoint(800, 425), 150, 25, false)); // Another platform
	secondPlayerTower.push_back(Platform(fPoint(800, 450), 150, 25, false)); // Another platform
	//platforms.push_back(Platform(fPoint(300, 300), 25, 200, 999, false)); // Another platform indestructible
	//platforms.push_back(Platform(fPoint(600, 150), 100, 20, 999, false)); // Another platform
	//300,300,25,200
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
	if (app->scene_intro->restartLevel) {
		platforms.clear();
		platforms.push_back(Platform(fPoint(300, 400), 180, 70, true)); 
		platforms.push_back(Platform(fPoint(800, 100), 150, 20, false));
		platforms.push_back(Platform(fPoint(500, 300), 100, 20, false));
		platforms.push_back(Platform(fPoint(100, 100), 100, 20, false));
		platforms.push_back(Platform(fPoint(900, 200), 100, 20, false));
		platforms.push_back(Platform(fPoint(200, 150), 100, 20, false));
		platforms.push_back(Platform(fPoint(700, 400), 100, 20, false));
		platforms.push_back(Platform(fPoint(200, 250), 100, 20, false));
		platforms.push_back(Platform(fPoint(600, 150), 100, 20, false));
		platforms.push_back(Platform(fPoint(400, 300), 100, 20, false));
		platforms.push_back(Platform(fPoint(200, 250), 100, 20, false));
		platforms.push_back(Platform(fPoint(600, 150), 100, 20, false));

		firstPlayerTower.clear();
		firstPlayerTower.push_back(Platform(fPoint(100, 275), 150, 25, false)); // Another platform
		firstPlayerTower.push_back(Platform(fPoint(100, 300), 150, 25, false)); // Another platform
		firstPlayerTower.push_back(Platform(fPoint(100, 325), 150, 25, false)); // Another platform
		firstPlayerTower.push_back(Platform(fPoint(100, 350), 150, 25, false)); // Another platform
		firstPlayerTower.push_back(Platform(fPoint(100, 375), 150, 25, false)); // Another platform
		firstPlayerTower.push_back(Platform(fPoint(100, 400), 150, 25, false)); // Another platform
		firstPlayerTower.push_back(Platform(fPoint(100, 425), 150, 25, false)); // Another platform
		firstPlayerTower.push_back(Platform(fPoint(100, 450), 150, 25, false)); // Another platform

		secondPlayerTower.clear();
		secondPlayerTower.push_back(Platform(fPoint(800, 275), 150, 25, false)); // Another platform
		secondPlayerTower.push_back(Platform(fPoint(800, 300), 150, 25, false)); // Another platform
		secondPlayerTower.push_back(Platform(fPoint(800, 325), 150, 25, false)); // Another platform
		secondPlayerTower.push_back(Platform(fPoint(800, 350), 150, 25, false)); // Another platform
		secondPlayerTower.push_back(Platform(fPoint(800, 375), 150, 25, false)); // Another platform
		secondPlayerTower.push_back(Platform(fPoint(800, 400), 150, 25, false)); // Another platform
		secondPlayerTower.push_back(Platform(fPoint(800, 425), 150, 25, false)); // Another platform
		secondPlayerTower.push_back(Platform(fPoint(800, 450), 150, 25, false)); // Another platform

		towerlive1 = 8;
		towerlive2 = 8;
		score1 = 0;
		score2 = 0;
	}



	//Calculate Frame Time:
	currentFrameTime = SDL_GetTicks() / 1000.0f; // Convert milliseconds to seconds
	float deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;  // Update lastFrameTime for the next frame

	const float maxDeltaTime = 1.0f / 10.0f; // Maximum allowed deltaTime
    #undef min
	deltaTime = std::min(deltaTime, maxDeltaTime);

	bool isBallOnPlatform = false;



	// Update platforms
	for (Platform& platform : platforms) {
		for (Body* body : bodies) {
			
			if (platform.checkCollision(*body)) {
				// The collision response is handled within the checkCollision method
				// Additional logic after collision (if necessary)
				if (platform.isAWatterPlatform)
				{
					body->isCollidingWithWater = true;
				}
				else
				{
					platform.applyDamage(5);
				}
				isBallOnPlatform = true;
			}
			
		}
	}
	
	for (Platform& platform : firstPlayerTower) 
	{
		for (Body* body : bodies) 
		{
			if (platform.checkCollision(*body)) 
			{
					// The collision response is handled within the checkCollision method
					// Additional logic after collision (if necessary)
				firstPlayerTower.pop_front();
				bodies.remove(body);
				score2++;
				towerlive1--;
				goto endLoop;
				
			}
		}
	}
	endLoop:
	for (Platform& platform : secondPlayerTower) 
	{
		for (Body* body : bodies) 
		{
			if (platform.checkCollision(*body)) 
			{
					// The collision response is handled within the checkCollision method
					// Additional logic after collision (if necessary)
				secondPlayerTower.pop_front();
				bodies.remove(body);
				score1++;
				towerlive2--;
				goto endLoop2;
				
			}
		}
	}
	endLoop2:
	

	for (Body* body : bodies) {
		for (Platform& platform : platforms) {
			// Only check collision with active platforms
			if (platform.health > 0 && platform.checkCollision(*body)) {
				// Handle collision with platform
				// If the body is a ball, mark it as inactive
				if (body->isBall) {  // Assuming you have an 'isBall' flag or similar
					body->active = false;
				}
			}
		}
	}

	// Remove inactive bodies or handle them accordingly
	bodies.remove_if([](Body* body) { return !body->active; });

	for (Body* body : bodies) {

		if ( !isBallOnPlatform) {
			// Establecer el booleano en falso ya que la bola ha salido de la plataforma
			body->isCollidingWithWater = false;
		}
	}


	// Handle collisions and physics for each body
	for (Body* body : bodies) {
		for (Platform& platform : platforms) {
			if (platform.checkCollision(*body)) {
				// The collision response is handled within the checkCollision method
				// Additional logic after collision (if necessary)
				
			}
		}
	}

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
		//This is for implementing the physics of the bodies 
		Body* body = *it;
		fPoint halfTimeVelocity;
		
		//This is for implementing the hydrodynamics of the bodies 
		if (body->isCollidingWithWater)
		{
			//Here is the ecuation of hydrodynamics
			
			if (body->counterForWatter == 0) { //initialize counter 
				counterForWater = (body->mass * body->acceleration.y);
			}
			float phi = counterForWater * cos(body->counterForWatter * 0.05f);  // Adjust the potential function
			int y = static_cast<int>(phi); //get new position
			body->position.y += y; // add the trayectory of the new position
			body->position.x += body->velocity.x * deltaTime;
			body->velocity.x += body->acceleration.x * deltaTime; //Update x position
			if (counterForWater < 0) //mantain the values of the cos stable to not get huge trayectories
			{
				tempCounterWatter = 1;
			}
			else if(counterForWater>= (body->mass * body->acceleration.y)/2)
			{
				tempCounterWatter = -1;
			}
			counterForWater += tempCounterWatter; 

			body->counterForWatter += 1; //add another position to the trayectory
		}
		else
		{
			
			tempCounterWatter = 1;
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
				bodies.remove(body);
				break;
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
		

	}

	
	lastFrameTime = currentFrameTime;

	return UPDATE_CONTINUE;
}

update_status ModulePhysics::PostUpdate()
{
	// In the PostUpdate method
	//draw the ground with a line
	int width, height;
	App->window->GetWindowSize(width, height);
	if (app->player->firstalive == true && app->player->secondalive == true) 
	{
		if (debug)
		{
			App->renderer->DrawLine(0, height - 290, width, height - 300, 255, 255, 255, 255);
		}
		else
		{
			App->renderer->DrawLine(0, height - 290, width, height - 300, 255, 255, 255, 0);
		}


		const std::list<Platform>& tower1 = App->physics->GetTower1();
		for (const Platform& platform : tower1)
		{
			SDL_Rect rect = { static_cast<int>(platform.position.x), static_cast<int>(platform.position.y),
							  static_cast<int>(platform.width), static_cast<int>(platform.height) };
			if (debug)
			{
				App->renderer->DrawQuad(rect, 255, 255, 255); // Red color for platforms
			}
			else
			{
				App->renderer->DrawQuad(rect, 255, 255, 255, 0); // Red color for platforms

			}
		}
		const std::list<Platform>& tower2 = App->physics->GetTower2();
		for (const Platform& platform : tower2)
		{
			SDL_Rect rect = { static_cast<int>(platform.position.x), static_cast<int>(platform.position.y),
							  static_cast<int>(platform.width), static_cast<int>(platform.height) };
			if (debug)
			{
				App->renderer->DrawQuad(rect, 255, 255, 255); // Red color for platforms
			}
			else
			{
				App->renderer->DrawQuad(rect, 255, 255, 255, 0); // Red color for platforms
			}
		}

		switch (tower1.size())
		{
		case 0:
			//You win the game
			towerlive1 = 0;
			break;
		case 1:
			App->renderer->Blit(towerTexture1, 35, 250);

			break;
		case 2:
			App->renderer->Blit(towerTexture2, 35, 250);
			break;
		case 3:
			App->renderer->Blit(towerTexture3, 35, 250);
			break;
		case 4:
			App->renderer->Blit(towerTexture4, 35, 250);
			break;
		case 5:
			App->renderer->Blit(towerTexture5, 35, 250);
			break;
		case 6:
			App->renderer->Blit(towerTexture6, 35, 250);
			break;
		case 7:
			App->renderer->Blit(towerTexture7, 35, 250);
			break;
		case 8:
			App->renderer->Blit(towerTexture8, 35, 250);
			break;
		default:
			break;
		}

		switch (tower2.size())
		{
		case 0:
			//You win the game
			towerlive2 = 0;
			break;
		case 1:
			App->renderer->Blit(towerTexture1, 735, 250);
			break;
		case 2:
			App->renderer->Blit(towerTexture2, 735, 250);
			break;
		case 3:
			App->renderer->Blit(towerTexture3, 735, 250);
			break;
		case 4:
			App->renderer->Blit(towerTexture4, 735, 250);
			break;
		case 5:
			App->renderer->Blit(towerTexture5, 735, 250);
			break;
		case 6:
			App->renderer->Blit(towerTexture6, 735, 250);
			break;
		case 7:
			App->renderer->Blit(towerTexture7, 735, 250);
			break;
		case 8:
			App->renderer->Blit(towerTexture8, 735, 250);
			break;
		default:
			break;
		}
		for (list<Body*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
		{

			Body* body = *it;
			App->renderer->Blit(ballTexture, body->position.x - 40, body->position.y - 50);

			if (debug)
			{
				App->renderer->DrawCircle(body->position.x, body->position.y, 20, 255, 0, 0);
			}

		}
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

	
	
	


	return UPDATE_CONTINUE;
}




// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");
	return true;
}
