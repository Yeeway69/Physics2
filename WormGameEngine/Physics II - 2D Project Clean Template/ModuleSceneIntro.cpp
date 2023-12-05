#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"



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
	backgroundTexture = App->textures->Load("Assets/fondofinal.png");
	crownTexture = App->textures->Load("Assets/crown.png");
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
	} 
	else
	{
		//Render loose screen
		if (app->player->firstalive == true) {
			//Player 1 winScreen
			app->physics->score1 = app->physics->firstPlayerTower.size();
			if (app->physics->score1 >=6)
			{
				App->renderer->Blit(crownTexture, 600, 300);
			}
			if (app->physics->score1 >= 4)
			{
				App->renderer->Blit(crownTexture, 500, 200);
			}
			App->renderer->Blit(crownTexture, 400, 300);
		}
		else
		{
			//Player 2 winScreen
			app->physics->score2 = app->physics->secondPlayerTower.size();
			if (app->physics->score2 >= 6)
			{
				App->renderer->Blit(crownTexture, 600, 300);
			}
			if (app->physics->score2 >= 4)
			{
				App->renderer->Blit(crownTexture, 500, 250);
			}
			App->renderer->Blit(crownTexture, 400, 300);
		}

		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) 
		{
			restartLevel = true;
		}
	}

	


	// Render platforms
	
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

			}
			
		}
	}

	//// Debug: Render platform positions
	//for (const Platform& platform : App->physics->GetPlatforms()) {
	//	SDL_Rect rect = { static_cast<int>(platform.position.x), static_cast<int>(platform.position.y),
	//					  static_cast<int>(platform.width), static_cast<int>(platform.height) };
	//	App->renderer->DrawQuad(rect, 0, 255, 0, 255, false); // Green color for platform positions

	//	
	//}

	return UPDATE_CONTINUE;
}
