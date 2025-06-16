#include "PhysicsWorld.h"

void Physics::PhysicsWorld::AddParticle(Particle* toAdd) {
	particles.push_back(toAdd);
	forceRegistry.Add(toAdd, &Gravity);
}


void Physics::PhysicsWorld::Update(float time) {
	UpdateParticleList();

	forceRegistry.UpdateForces(time);

	for (std::list<Particle*>::iterator p = particles.begin(); p != particles.end(); ++p)
	{
		(*p)->Update(time);
	}
}

void Physics::PhysicsWorld::UpdateParticleList() {
	particles.remove_if([](Particle* p) { return p->IsDestroyed(); });
}