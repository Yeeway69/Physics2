#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"
//include c++ standard libraries here
#include <list>
using namespace std;

const float G = -9.8f;
typedef p2Point<float> fPoint;

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
    float mass;
    float elasticity; // coefficient of restitution (bounce factor)
    float friction; // coefficient of friction
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

    fPoint NGUL(float m1, float m2, float r1)
    {
        return { (G * m1 * m2) / (r1 * r1), (G * m1 * m2) / (r1 * r1) };
    }


    //for framrate control
    FRAMERATE_MODE currentMode = FRM_FIXED; // Default mode
    const float fixedFrameTime = 1.0f / 60.0f; // for 60 FPS
    float lastFrameTime = 0.0f;
    float currentFrameTime = 0.0f; // This will be updated during runtime

    // Debug Stats
    int targetFPS = 60;  // Default target FPS
    float targetFrameTime = fixedFrameTime;
    int currentFPS = 0;  // This will be updated during runtime
    

private:

    //angle in radians
    float cannonAngle;  // Angle in degrees
    float cannonPower;  // Firing power

    Body* player= new Body(); //player tehe mnn 

    float groundHeight; // Y coordinate of the ground

    float groundwidth; // X coordinate of the ground


    list<Body*> bodies; // List of all physics bodies

    const fPoint gravity = { 0.0f, 9.81f }; // gravitational acceleration (assuming Y is up)
    Body* star = new Body();
    Body* planet1 = new Body();
    Body* planet2 = new Body();
    Body* object1 = new Body();
    Body* object2 = new Body();
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
