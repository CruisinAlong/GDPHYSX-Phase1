#pragma once
#include "Physics/MyParticle.h"
#include "ModelLoader.h"

namespace Physics {
    class RenderParticle {
    public:
        Physics::Particle* PhysicsParticle;
        ModelLoader* RenderObject;
        Physics::MyVector Color;
        glm::vec3 Scale;

        RenderParticle(Physics::Particle* p, ModelLoader* obj)
            : PhysicsParticle(p), RenderObject(obj), Color(1, 1, 1) {
        }

        RenderParticle(Physics::Particle* p, ModelLoader* obj, Physics::MyVector c, glm::vec3 scale)
            : PhysicsParticle(p), RenderObject(obj), Color(c),  Scale(scale){
        }

        void Draw(GLuint shaderProgram, float scale, float x_rot);
    };
}
