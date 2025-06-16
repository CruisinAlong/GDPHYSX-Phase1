#pragma once
#include "ForceGenerator.h"
namespace Physics {
	class DragForceGenerator : public ForceGenerator {
	private:
		float k1 = 0.74f;
		float k2 = 0.57f;
	public:
		DragForceGenerator() {};
		DragForceGenerator(float k1, float k2) : k1(k1), k2(k2) {}

		void UpdateForce(Particle* particle, float time);
	};
}