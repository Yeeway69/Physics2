#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include<stdio.h>


using namespace std;

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	this->app = app;
	graphics = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	scoreFont = App->fonts->Load("Assets/textocosa4.png", lookupTable, 2);

	//Load Textures
	backgroundTexture = App->textures->Load("Assets/fondofinal.png");
	crownTexture = App->textures->Load("Assets/crown.png");
	player1WinsTexture = App->textures->Load("Assets/ScreenEnding1.png");
	player2WinsTexture = App->textures->Load("Assets/ScreenEnding2.png");
	platformTexture = App->textures->Load("Assets/plataforma.png");
	waterTexture = App->textures->Load("Assets/water.png");

	App->audio->PlayMusic("Assets/music.wav", 0.0f);

	App->renderer->camera.x = App->renderer->camera.y = 0;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	
	if (app->player->firstalive == true && app->player->secondalive == true)
	{
		restartLevel = false;
		App->renderer->Blit(backgroundTexture, 0, 0);
	
		//Render Platforms
		const std::list<Platform>& platforms = App->physics->GetPlatforms();
		for (const Platform& platform : platforms) {
			if (platform.health > 0) {
				SDL_Rect rect = { static_cast<int>(platform.position.x), static_cast<int>(platform.position.y),
								  static_cast<int>(platform.width), static_cast<int>(platform.height) };
				if (app->physics->debug)
				{
					App->renderer->DrawQuad(rect, 255, 0, 0, 255); // Red color for platforms
				}
				else
				{
					App->renderer->DrawQuad(rect, 255, 0, 0, 0); // Red color for platforms
				}
				if (!platform.isAWatterPlatform)
				{
					App->renderer->Blit(platformTexture, platform.position.x - 25, platform.position.y - 67);

				}
				else
				{
					App->renderer->Blit(waterTexture, platform.position.x, platform.position.y);
				}


			}
		}
	} 
	else
	{
		//Render loose screen
		if (app->player->firstalive == true) 
		{
			//Player 1 winScreen
			App->renderer->Blit(player1WinsTexture, 0, 0);
			app->physics->score1 = app->physics->firstPlayerTower.size();
			if (app->physics->score1 >=6)
			{
				App->renderer->Blit(crownTexture, 850, 450);
			}
			if (app->physics->score1 >= 4)
			{
				App->renderer->Blit(crownTexture, 700, 400);
			}
			App->renderer->Blit(crownTexture, 550, 450);
		
			
		}
		else
		{
			//Player 2 winScreen
			App->renderer->Blit(player2WinsTexture, 0, 0);

			app->physics->score2 = app->physics->secondPlayerTower.size();
			if (app->physics->score2 >= 6)
			{
				App->renderer->Blit(crownTexture, 350 , 450);
			}
			if (app->physics->score2 >= 4)
			{
				App->renderer->Blit(crownTexture, 200, 400);
			}
			App->renderer->Blit(crownTexture, 50, 450);
		}

		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) 
		{
			restartLevel = true;
		}
	}

	


	// Render platforms
	
	

	//// Debug: Render platform positions
	//for (const Platform& platform : App->physics->GetPlatforms()) {
	//	SDL_Rect rect = { static_cast<int>(platform.position.x), static_cast<int>(platform.position.y),
	//					  static_cast<int>(platform.width), static_cast<int>(platform.height) };
	//	App->renderer->DrawQuad(rect, 0, 255, 0, 255, false); // Green color for platform positions

	//	
	//}

	return UPDATE_CONTINUE;
}
