#pragma once
#include "p2Point.h"

class GameObject
{
public:
    GameObject(p2Point<float> position, p2Point<float> velocity, float mass);
    ~GameObject();

    void ApplyForce(p2Point<float> force); // Apply an external force to the object

    // Getter and Setter methods for position, velocity, etc.
    void SetPosition(p2Point<float> pos);
    p2Point<float> GetPosition() const;
    void SetVelocity(p2Point<float> vel);
    p2Point<float> GetVelocity() const;

    // Update method to update physics properties over time
    void Update(float dt);

private:
    p2Point<float> position;  // Current position of the object
    p2Point<float> velocity;  // Current velocity of the object
    p2Point<float> acceleration;  // Current acceleration of the object
    float mass;  // Mass of the object

    static const p2Point<float> gravity;  // Gravity force applied to all objects
};
