#include "DragForceGenerator.h"

namespace Physics {
	void Physics::DragForceGenerator::UpdateForce(Particle* particle, float time)
	{
		MyVector force = MyVector(0, 0, 0);
		MyVector currV = particle->Velocity;

		float mag = currV.Magnitude();
		if (mag <= 0.0f) return;

		float dragF = (k1 * mag) + (k2 * mag);
		MyVector dir = currV.Normalize();

		particle->AddForce(dir * -dragF);
	}
}
