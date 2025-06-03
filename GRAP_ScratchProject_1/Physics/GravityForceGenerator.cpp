#include "GravityForceGenerator.h"
namespace Physics {
	void GravityForceGenerator::UpdateForce(Particle* particle, float time)
	{
		if (particle->IsDestroyed() || particle->mass <= 0.0f)
			return;
		MyVector force = Gravity * particle->mass;
		particle->AddForce(force);
	}
}