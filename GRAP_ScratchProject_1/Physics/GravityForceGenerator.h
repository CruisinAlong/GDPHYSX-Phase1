#pragma once

#include "ForceGenerator.h"

namespace Physics {
	class GravityForceGenerator : public ForceGenerator {
	private:
		MyVector Gravity = MyVector(0.0f, -9.81f, 0.0f);

	public:
		GravityForceGenerator(MyVector gravity) : Gravity(gravity) {}
		void UpdateForce(Particle* particle, float time) override;
	};
}