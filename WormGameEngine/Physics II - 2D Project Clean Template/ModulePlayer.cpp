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
	char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	scoreFont = App->fonts->Load("Assets/textocosa4.png", lookupTable, 2);

	ballused1 = 0;
	ballused2 = 0;
	//Init textures
	player1TextureUp = App->textures->Load("Assets/mortar1.png");
	player1TextureLeft = App->textures->Load("Assets/mortar2.png");
	player1TextureRight = App->textures->Load("Assets/mortar3.png");
	player2TextureUp = App->textures->Load("Assets/mortarn1.png");
	player2TextureLeft = App->textures->Load("Assets/mortarn2.png");
	player2TextureRight = App->textures->Load("Assets/mortarn3.png");



	// Example: Initialize a player body (this is just for demonstration and can be adapted as needed)
	player->position = { 200.0f, SCREEN_HEIGHT - 600}; // starting position
	player->velocity = { 0.0f, 0.0f }; // starting velocity
	player->acceleration = gravity; // affected by gravity
	player->mass = 1.0f; // arbitrary mass
	player->elasticity = 0.5f; // some bounce
	player->friction = 0.1f; // some friction
	//app->physics->bodies.push_back(player);

	//firing
	cannonAngle = 45.0f;  // Default to 45 degrees
	cannonPower = 60.0f;   // Default power
	LOG("Loading player");

	//player2
	player2->position = { 850.0f,SCREEN_HEIGHT - 600 };
	player2->velocity = { 0.0f, 0.0f }; // starting velocity
	player2->acceleration = gravity; // affected by gravity
	player2->mass = 1.0f; // arbitrary mass
	player2->elasticity = 0.5f; // some bounce
	player2->friction = 0.1f; // some friction
	cannonAngle2 = 135.0f;  // Default to 45 degrees
	cannonPower2 = 60.0f;   // Default power
	//app->physics->bodies.push_back(player2);
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
	
	if (app->scene_intro->restartLevel) 
	{
		player->position.x = 200;
		player2->position.x = 850;
		firstalive = true;
		secondalive = true;
		firstplayer = true;
		cannonAngle = 45.0f; 
		cannonPower = 60.0f;  
		cannonAngle2 = 135.0f; 
		cannonPower2 = 60.0f;  
		player1TextureUp = App->textures->Load("Assets/mortar1.png");
		player1TextureLeft = App->textures->Load("Assets/mortar2.png");
		player1TextureRight = App->textures->Load("Assets/mortar3.png");
		player2TextureUp = App->textures->Load("Assets/mortarn1.png");
		player2TextureLeft = App->textures->Load("Assets/mortarn2.png");
		player2TextureRight = App->textures->Load("Assets/mortarn3.png");
	}

	player->position.y = SCREEN_HEIGHT - 300-(25*app->physics->firstPlayerTower.size());
	player2->position.y = SCREEN_HEIGHT - 300 - (25 * app->physics->secondPlayerTower.size());

	
	if (firstplayer == true) 
	{
		if (firstalive == true && secondalive == true) {
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && cannonAngle < 170)
			{
				cannonAngle += 1.0f;
			}
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && cannonAngle > 10)
			{
				cannonAngle -= 1.0f;
			}
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && cannonPower < 150)  // Adjust as needed based on your input handling
			{
				cannonPower += 0.5f;
			}
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && cannonPower > 0)  // Adjust as needed based on your input handling
			{
				cannonPower -= 0.5f;
			}


			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && player->position.x > 120) {
				player->position.x -= 3;
			}
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && player->position.x < 230) {
				player->position.x += 3;
			}
		}
		
	}
	else {
		if (secondalive == true && firstalive == true) {
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && cannonAngle2 < 170)
			{
				cannonAngle2 += 1.0f;
			}
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && cannonAngle2 > 10)
			{
				cannonAngle2 -= 1.0f;
			}
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && cannonPower2 < 150)  // Adjust as needed based on your input handling
			{
				cannonPower2 += 0.5f;
			}
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && cannonPower2 > 0)  // Adjust as needed based on your input handling
			{
				cannonPower2 -= 0.5f;
			}
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && player2->position.x > 820) {
				player2->position.x -= 3;
			}
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && player2->position.x < 930) {
				player2->position.x += 3;
			}
		}
		
	}
	

	

	const fPoint cannonPosition = { player->position.x, player->position.y - 10 };
	const fPoint cannonPosition2 = { player2->position.x, player2->position.y - 10 };
	


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN &&!hasShot) // Listen for SPACEBAR press
	{
		if (firstplayer == true) {
			if (firstalive == true && secondalive == true) {
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
				firstplayer = false;
				ballused1++;
			}
			
		}
		else {
			if (secondalive == true && firstalive == true) {
				Body* cannonball2 = new Body();
				cannonball2->position = cannonPosition2;
				cannonball2->velocity = { -2.0f, -4.0f }; // Initial velocity (adjust as needed)
				cannonball2->acceleration = gravity; // Affected by gravity
				cannonball2->mass = 1.0f; // Arbitrary mass
				cannonball2->elasticity = 0.5f; // Some bounce
				cannonball2->friction = 0.1f; // Some friction
				app->physics->bodies.push_back(cannonball2);

				float radianAngle = DEGTORAD(cannonAngle2);  // Convert angle  to radians
				cannonball2->velocity.x = cannonPower2 * cosf(radianAngle);
				cannonball2->velocity.y = -cannonPower2 * sinf(radianAngle);  // Negative because y is up
				firstplayer = true;
				ballused2++;
			}
			
		}
		

		

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
		if (firstplayer == true) {
			if (firstalive == true && secondalive == true) {
				float radianAngle = DEGTORAD(-cannonAngle);
				int endX = player->position.x + static_cast<int>(100 * std::cosf(radianAngle));
				int endY = player->position.y + static_cast<int>(100 * std::sinf(radianAngle));

				App->renderer->DrawLine(endX, endY, player->position.x, player->position.y, 255, 255, 255);
			}
			
		}
		else {
			if (firstalive == true && secondalive == true) {
				float radianAngle = DEGTORAD(-cannonAngle2);
				int endX = player2->position.x + static_cast<int>(100 * std::cosf(radianAngle));
				int endY = player2->position.y + static_cast<int>(100 * std::sinf(radianAngle));

				App->renderer->DrawLine(endX, endY, player2->position.x, player2->position.y, 255, 255, 255);
			}
			
		}
		
	}
	
	if (app->physics->towerlive1 == 0 || app->physics->towerlive2 == 0) 
	{
		player1TextureUp = App->textures->Load("Assets/.png");
		player1TextureLeft = App->textures->Load("Assets/.png");
		player1TextureRight = App->textures->Load("Assets/.png");
		player2TextureUp = App->textures->Load("Assets/.png");
		player2TextureLeft = App->textures->Load("Assets/.png");
		player2TextureRight = App->textures->Load("Assets/.png");
		if (app->physics->towerlive1 == 0) 
		{
			firstalive = false;
		}
		else
		{
			secondalive = false;
		}
	}

	if (app->player->firstalive == true && app->player->secondalive == true)
	{
		//This is the power var 
		App->renderer->DrawGrowingRectangle(SCREEN_WIDTH * 0.05f, SCREEN_HEIGHT / 2, cannonPower, 0, 152, 70, 255, true);
		App->renderer->DrawGrowingRectangle(SCREEN_WIDTH * 0.97f, SCREEN_HEIGHT / 2, cannonPower2, 0, 152, 70, 255, true);

		//rendering
		float radianAngle = DEGTORAD(cannonAngle);
		if (cosf(radianAngle) > 0.4f)
		{
			App->renderer->Blit(player1TextureRight, player->position.x - 80, player->position.y - 115);
		}
		else if (cosf(radianAngle) < -0.4f)
		{
			App->renderer->Blit(player1TextureLeft, player->position.x - 80, player->position.y - 115);
		}
		else
		{
			App->renderer->Blit(player1TextureUp, player->position.x - 80, player->position.y - 115);
		}
		float radianAngle2 = DEGTORAD(cannonAngle2);
		if (cosf(radianAngle2) > 0.4f)
		{
			App->renderer->Blit(player2TextureLeft, player2->position.x - 80, player2->position.y - 115);
		}
		else if (cosf(radianAngle2) < -0.4f)
		{
			App->renderer->Blit(player2TextureRight, player2->position.x - 80, player2->position.y - 115);
		}
		else
		{
			App->renderer->Blit(player2TextureUp, player2->position.x - 80, player2->position.y - 115);
		}
	}

	sprintf_s(scoreText, 10, "%7d", score);
	sprintf_s(highscoreText, 10, "%7d", highscore);
	sprintf_s(livesText, 10, "%7d", lives);

	//App->fonts->BlitText(1, 15, scoreFont, scoreText);

	//App->fonts->BlitText(2, 2, scoreFont, "player1");
	App->fonts->BlitText(2, 2, scoreFont, "highscore");
	/*App->fonts->BlitText(96, 15, scoreFont, highscoreText);
	App->fonts->BlitText(180, 2, scoreFont, "lives x ");
	App->fonts->BlitText(192, 2, scoreFont, livesText);*/

	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{
	return UPDATE_CONTINUE;
}










