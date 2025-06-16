#pragma once
#include <list>
#include "MyParticle.h"
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"

namespace Physics {
	class PhysicsWorld {
	public:
		std::list<Particle*> particles;
		ForceRegistry forceRegistry;

		void AddParticle(Particle* toAdd);
		void Update(float time);


	private:
		void UpdateParticleList();

		GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0.0f, -9.81f, 0.0f));
	};
}