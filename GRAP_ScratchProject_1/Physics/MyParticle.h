#pragma once
#include "MyVector.h"
namespace Physics {
	class Particle
	{
	public:
		float mass = 0.0f;
		MyVector Position;
		MyVector Velocity;
		MyVector Acceleration;
		float lifespan = -1.0f;

	protected:
		void UpdatePosition(float time);
		void UpdateVelocity(float time);


	protected:
		bool isDestroyed = false;

	protected:
		float damping = 1.0f;
		MyVector accumulatedForce = MyVector(0, 0, 0);

	public:
		void Update(float time);
		void Destroy();
		bool IsDestroyed() const { return isDestroyed; }
		void AddForce(MyVector force);
		void ResetForce();
	};
}
