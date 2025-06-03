#pragma once
#include "Physics/MyParticle.h"
#include "ModelLoader.h"

namespace Physics {
    class RenderParticle {
    public:
        Physics::Particle* PhysicsParticle;
        ModelLoader* RenderObject;
        Physics::MyVector Color;

        RenderParticle(Physics::Particle* p, ModelLoader* obj)
            : PhysicsParticle(p), RenderObject(obj), Color(1, 1, 1) {
        }

        RenderParticle(Physics::Particle* p, ModelLoader* obj, Physics::MyVector c)
            : PhysicsParticle(p), RenderObject(obj), Color(c) {
        }

        void Draw(GLuint shaderProgram, float scale, float x_rot);
    };
}
