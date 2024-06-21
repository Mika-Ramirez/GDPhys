#pragma once
#include "MyVector.h"

namespace P6 {

	class PhysicsParticles : public MyVector
	{
	public:
		float mass = 0;
		MyVector Velocity;
		MyVector Position;
		MyVector Acceleration;


	protected:
		void UpdatePosition(float time);
		void UpdateVelocity(float time);

		bool isDestroyed = false;

	public:
		void update(float time);

		void Destroy();

		bool IsDestroyed() {
			return isDestroyed;
		}

	};
}
