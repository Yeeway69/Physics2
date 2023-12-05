#pragma once
#include "Module.h"
#include "Animation.h"
#include "ModulePhysics.h"
#include "Globals.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"
#include <list>
using namespace std;


class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();


public:
	float cannonAngle;  // Angle in degrees
	float cannonPower = 6;  // Firing power
	float cannonAngle2;  // Angle in degrees
	float cannonPower2 = 6;  // Firing power
	int ballused1;
	int ballused2;

	bool firstalive = true;
	bool secondalive = true;
	

private:
	Body* player = new Body();
	Body* player2 = new Body();
	Application* app;


	SDL_Texture* player1TextureUp;
	SDL_Texture* player1TextureRight;
	SDL_Texture* player1TextureLeft;
	SDL_Texture* player2TextureUp;
	SDL_Texture* player2TextureRight;
	SDL_Texture* player2TextureLeft;
	
	int frameCounter = 0;
	int frameCooldown = 50;
	bool hasShot = false;
	bool firstplayer = true;

};