#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
	graphics = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

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
	// Render platforms
	const std::list<Platform>& platforms = App->physics->GetPlatforms();
	for (const Platform& platform : platforms) {
		if (platform.health > 0) {
			SDL_Rect rect = { static_cast<int>(platform.position.x), static_cast<int>(platform.position.y),
							  static_cast<int>(platform.width), static_cast<int>(platform.height) };
			App->renderer->DrawQuad(rect, 255, 0, 0, 255); // Red color for platforms
		}
	}

	// Debug: Render platform positions
	for (const Platform& platform : App->physics->GetPlatforms()) {
		SDL_Rect rect = { static_cast<int>(platform.position.x), static_cast<int>(platform.position.y),
						  static_cast<int>(platform.width), static_cast<int>(platform.height) };
		App->renderer->DrawQuad(rect, 0, 255, 0, 255, false); // Green color for platform positions

		
	}

	return UPDATE_CONTINUE;
}
