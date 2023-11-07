#include "GameObject.h"

const p2Point<float> GameObject::gravity = { 0, 9.81f };  // Assuming Y is upward, change sign if downward

GameObject::GameObject(p2Point<float> position, p2Point<float> velocity, float mass)
{
    this->position.x = position.x;  // Assign values separately
    this->position.y = position.y;

    this->velocity.x = velocity.x;
    this->velocity.y = velocity.y;

    this->mass = mass;
    this->acceleration = { 0, 0 };  // Initialize to zero using assignment
}

GameObject::~GameObject() {}

void GameObject::ApplyForce(p2Point<float> force)
{
    // Using Newton's second law F = ma, so a = F/m
    acceleration.x += force.x / mass;
    acceleration.y += force.y / mass;
}



void GameObject::SetPosition(p2Point<float> pos) { position = pos; }
p2Point<float> GameObject::GetPosition() const { return position; }
void GameObject::SetVelocity(p2Point<float> vel) { velocity = vel; }
p2Point<float> GameObject::GetVelocity() const { return velocity; }

void GameObject::Update(float dt)
{
    // Apply gravity to acceleration
    acceleration.y += gravity.y;

    // Update position and velocity using the kinematic equations
    position.x += velocity.x * dt + 0.5f * acceleration.x * dt * dt;
    position.y += velocity.y * dt + 0.5f * acceleration.y * dt * dt;
    velocity.x += acceleration.x * dt;
    velocity.y += acceleration.y * dt;

    // Reset acceleration for the next frame
    acceleration = { 0, 0 };
}
