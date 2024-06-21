#include "PhysicsParticles.h"

using namespace P6;

void PhysicsParticles::UpdatePosition(float time)
{

	float newTime = time * time;
	this->Position = this->Position + (this->Velocity.ScalarMultiplication(time)) + ((this->Acceleration.ScalarMultiplication(newTime)).ScalarMultiplication(0.5f));
}

void PhysicsParticles::UpdateVelocity(float time)
{
	this->Velocity = this->Velocity + (this->Acceleration.ScalarMultiplication(time));
}

void P6::PhysicsParticles::update(float time)
{
	this->UpdatePosition(time);
	this->UpdateVelocity(time);
}

void P6::PhysicsParticles::Destroy()
{
	this->isDestroyed = true;		
}
