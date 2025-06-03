#include "RenderParticle.h"

namespace Physics {

    void RenderParticle::Draw(GLuint shaderProgram, float scale, float x_rot) {
        if (!this->PhysicsParticle->IsDestroyed()) {
            this->RenderObject->Color = (glm::vec3)this->Color;
            this->RenderObject->Position = (glm::vec3)this->PhysicsParticle->Position;
            this->RenderObject->Draw(shaderProgram, scale, x_rot);
        }
    }

}
