#include "MyParticle.h"
#include "MyVector.h"
#include "cmath"

using namespace Physics; 

void Particle::UpdatePosition(float time) {
    Position = Position + (Velocity * time) + (Acceleration * (0.5f * (time * time)));
}

void Particle::UpdateVelocity(float time) {

	Acceleration += accumulatedForce * (1.0f / mass);
	Velocity = Velocity + (Acceleration * time);
	Velocity = Velocity * powf(damping, time);
}

void Physics::Particle::AddForce(MyVector force)
{
	accumulatedForce = accumulatedForce + force;
}

void Physics::Particle::ResetForce()
{
	accumulatedForce = MyVector(0, 0, 0);
	Acceleration = MyVector(0, 0, 0);
}

void Particle::Update(float time) {
    this->UpdatePosition(time);
	this->UpdateVelocity(time);

	this->ResetForce();
	if (lifespan > 0.0f) {
		lifespan -= time;
		if (lifespan <= 0.0f) {
			this->Destroy();
		}
	}
}

void Particle::Destroy() {
	isDestroyed = true;
}
