#include "PhysicsWorld.h"

void P6::PhysicsWorld::AddParticle(PhysicsParticles* toAdd)
{
	Particles.push_back(toAdd);
}

void P6::PhysicsWorld::Update(float time)
{
	UpdateParticleList();

	for (std::list<PhysicsParticles*>::iterator p = Particles.begin();


		p != Particles.end();

		p++

		)
	{
		(*p)->update(time);

	}

	
}

void P6::PhysicsWorld::UpdateParticleList()
{
	Particles.remove_if(
		[](PhysicsParticles* p) {
			return p->IsDestroyed();
		}
	);
}
