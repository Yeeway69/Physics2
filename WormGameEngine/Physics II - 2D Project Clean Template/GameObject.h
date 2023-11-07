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

    // Aerodynamic properties
    float area;            // Reference area for drag and lift calculations
    float dragCoefficient; // Coefficient of drag
    float liftCoefficient; // Coefficient of lift
    float angleOfAttack;   // Angle relative to the airflow

    void applyAerodynamicForces() {
        // Calculate velocity relative to the air
        // Use your existing velocity vector

        // Calculate drag force
        float dragForceMagnitude = 0.5f * airDensity * velocity.LengthSquared() * dragCoefficient * referenceArea;
        p2Point<float> dragForce = -dragForceMagnitude * velocity.Normalized();

        // Calculate lift force
        // Assuming 'up' is a unit vector perpendicular to the velocity direction
        p2Point<float> up(0.0f, 1.0f);
        float liftForceMagnitude = 0.5f * airDensity * velocity.LengthSquared() * liftCoefficient * referenceArea;
        p2Point<float> liftForce = liftForceMagnitude * up;

        // Apply forces to the object
        // This would add to any existing force that's been calculated
        ApplyForce(dragForce);
        ApplyForce(liftForce);
    }


    // Update method to update physics properties over time
    void Update(float dt);

    
private:
    p2Point<float> position;  // Current position of the object
    p2Point<float> velocity;  // Current velocity of the object
    p2Point<float> acceleration;  // Current acceleration of the object
    float mass;  // Mass of the object

    static const p2Point<float> gravity;  // Gravity force applied to all objects
};
