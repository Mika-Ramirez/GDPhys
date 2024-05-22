#pragma once
#include "MyVector.h"

namespace P6 {

	class PhysicsParticles 
	{
	public:
		float mass = 0;

		MyVector Position;
		MyVector Velocity;
		MyVector Acceleration;

		PhysicsParticles();

	protected:
		void UpdatePosition(float time);
		void UpdateVelocity(float time);

	public:
		void Update(float time);

	};
}