#include "ForceRegistry.h"

namespace Physics {
	void Physics::ForceRegistry::Add(Particle* particle, ForceGenerator* generator)
	{
		ParticleForceRegistry toAdd;
		toAdd.particle = particle;
		toAdd.forceGenerator = generator;
		Registry.push_back(toAdd);
	}

	void ForceRegistry::Remove(Particle* particle, ForceGenerator* generator)
	{
		Registry.remove_if([particle, generator](ParticleForceRegistry registry) {
			return registry.particle == particle && registry.forceGenerator == generator;
			});
	}

	void ForceRegistry::Clear()
	{
		Registry.clear();
	}

	void ForceRegistry::UpdateForces(float time)
	{
		for (std::list<ParticleForceRegistry>::iterator i = Registry.begin(); i != Registry.end(); ++i)
		{
			i->forceGenerator->UpdateForce(i->particle, time);
		}
	}

	
}
