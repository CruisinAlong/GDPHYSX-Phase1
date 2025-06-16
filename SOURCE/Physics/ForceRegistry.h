#pragma once

#include "MyParticle.h"
#include "ForceGenerator.h"
#include <list>
namespace Physics {
	class ForceRegistry {
	public: 
		void Add(Particle* particle, ForceGenerator* generator);
		void Remove(Particle* particle, ForceGenerator* generator);
		void Clear();
		void UpdateForces(float time);
	protected:
		struct ParticleForceRegistry {
			Particle* particle;
			ForceGenerator* forceGenerator;

		};

		std::list<ParticleForceRegistry> Registry;

	};
}
