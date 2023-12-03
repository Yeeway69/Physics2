#pragma once
#include "Module.h"
#include "Animation.h"
#include "ModulePhysics.h"
#include "Globals.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"
#include <list>
#include <chrono>
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



private:
	Body* player = new Body();
	Application* app;
	
	int frameCounter = 0;
	int frameCooldown = 50;
	bool hasShot = false;
};