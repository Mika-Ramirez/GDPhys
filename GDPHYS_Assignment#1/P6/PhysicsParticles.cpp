#include "PhysicsParticles.h"

using namespace P6;

P6::PhysicsParticles::PhysicsParticles()
{
	this->Acceleration = MyVector(0, 0, 0);
	this->Velocity = MyVector(0, 0, 0);
	this->Position = MyVector(0, 0, 0);

}

void P6::PhysicsParticles::UpdatePosition(float time)
{
	this->Position = (this->Velocity * time) + ((this->Acceleration * time * time) * (1.f / 2.f)) + this->Position;
}

void P6::PhysicsParticles::UpdateVelocity(float time)
{
	this->Velocity = (this->Acceleration * time) + this->Velocity;
}

void P6::PhysicsParticles::Update(float time)
{
	this->UpdatePosition(time);
	this->UpdateVelocity(time);
}