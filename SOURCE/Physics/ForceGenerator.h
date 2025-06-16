#pragma once
#include "MyParticle.h"
namespace Physics {
	class ForceGenerator {
		public:
			virtual void UpdateForce(Particle* p, float time) {
				p->AddForce(MyVector(0, 0, 0));
			}

	};
}