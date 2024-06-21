#include <list>
#include "PhysicsParticles.h"

#pragma once
namespace P6 {
	class PhysicsWorld
	{
	public:
		std::list<PhysicsParticles*> Particles;


		void AddParticle(PhysicsParticles* toAdd);
		void Update(float time);

	private:
		void UpdateParticleList();



	};
}
