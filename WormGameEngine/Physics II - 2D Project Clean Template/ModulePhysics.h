#pragma once
#include <algorithm>
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
    bool isCollidingWithWater = false; //A boolean that checks if the object is colliding with water
    int counterForWatter; //A counter for the trayectory of the watter
    float radius; // Radius of the ball
    float width; // Width of the platform
    float height; // Height of the platform
    bool wasOnPlatform;
    bool active = true;
    bool isAWatterBall = false;
    bool isBall = false;


};



struct Platform {
    fPoint position;
    float width, height;
    int health;
    bool isAWatterPlatform;

    Platform(fPoint pos, float w, float h, bool waterPlatform) : position(pos), width(w), height(h), health(20), isAWatterPlatform(waterPlatform) {}

    // Add collision checking method if needed
    bool Platform::checkCollision(Body& ball) {
        float ballRadius = 10.0f; // The radius of the ball

        // Inside your Platform::checkCollision method

        float closestX = (ball.position.x < position.x) ? position.x :
            (ball.position.x > position.x + width) ? position.x + width :
            ball.position.x;

        float closestY = (ball.position.y < position.y) ? position.y :
            (ball.position.y > position.y + height) ? position.y + height :
            ball.position.y;


        // Calculate the distance between the ball's center and this closest point
        float distanceX = ball.position.x - closestX;
        float distanceY = ball.position.y - closestY;

        // If the distance is less than the ball's radius, we have a collision
        if ((distanceX * distanceX + distanceY * distanceY) < (ballRadius * ballRadius)) {
            // Collision detected, now determine the response
            if (std::abs(distanceX) > std::abs(distanceY)) {
                // Horizontal collision, reverse X velocity
                if (!isAWatterPlatform) {
                    ball.velocity.x *= -1;
                }

            }
            else {
                // Vertical collision, reverse Y velocity
                ball.velocity.y *= -1;
            }

            return true;
        }

        return false;
    }


    void applyDamage(int damage) {
        health -= damage;
        if (health <= 0) {
            // Handle destruction
        }
    }
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

    const std::list<Platform>& GetPlatforms() const {
        return platforms;
    } 
    const std::list<Platform>& GetTower1() const {
        return firstPlayerTower;
    } 
    const std::list<Platform>& GetTower2() const {
        return secondPlayerTower;
    }

    int counterForWater;
    int tempCounterWatter = 1;
    std::list<Platform> firstPlayerTower;
    std::list<Platform> secondPlayerTower;
    int counterForRenderingTower = 0;
    int counterForRenderingTower2 = 0;

private:

    //angle in radians
    float cannonAngle;  // Angle in degrees
    float cannonPower;  // Firing power


    float groundHeight; // Y coordinate of the ground

    float groundwidth; // X coordinate of the ground

    bool debug;

    std::list<Platform> platforms; // List of all platforms

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
