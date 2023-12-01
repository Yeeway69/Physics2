#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"
//include c++ standard libraries here
#include <list>
using namespace std;


typedef p2Point<float> fPoint;
const fPoint gravity = { 0.0f, 9.81f }; // gravitational acceleration (assuming Y is up)

enum FRAMERATE_MODE
{
    FRM_FIXED,
    FRM_VARIABLE,
    FRM_SEMI_FIXED
};

struct Body {
    fPoint position;
    fPoint velocity;
    fPoint acceleration;
    float bodyAngle;
    float mass;
    float elasticity; // coefficient of restitution (bounce factor)
    float friction; // coefficient of friction
    bool isCollidingWithWater;
    int counterForWatter = 0;
};


class ModulePhysics : public Module
{
public:
    ModulePhysics(Application* app, bool start_enabled = true);
    ~ModulePhysics();

    bool Start();
    update_status PreUpdate();
    update_status PostUpdate();
    bool CleanUp();

    void UpdateWindowTitle();  // Add this line

    update_status PreUpdate(float deltaTime);  // Notice the float argument

public:
    //for framrate control
    FRAMERATE_MODE currentMode = FRM_FIXED; // Default mode
    const float fixedFrameTime = 1.0f / 60.0f; // for 60 FPS
    float lastFrameTime = 0.0f;
    float currentFrameTime = 0.0f; // This will be updated during runtime

    // Debug Stats
    int targetFPS = 60;  // Default target FPS
    float targetFrameTime = fixedFrameTime;
    int currentFPS = 0;  // This will be updated during runtime
    Body* player = new Body();
    list<Body*> bodies; // List of all physics bodies

    Application* app;
    int counterForWater;

private:

    //angle in radians
    float cannonAngle;  // Angle in degrees
    float cannonPower;  // Firing power


    float groundHeight; // Y coordinate of the ground

    float groundwidth; // X coordinate of the ground

    bool debug;

    // Apply a force to a body
    void ApplyForce(Body& body, const fPoint& force);

    enum IntegrationScheme
    {
        EULER,
        SYMPLECTIC_EULER,
        VELOCITY_VERLET
    };

    IntegrationScheme currentScheme;


};
