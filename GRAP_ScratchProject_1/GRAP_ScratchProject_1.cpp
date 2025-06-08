#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include "OrthoCamera.h"
#include "ModelLoader.h"
#include "Physics/MyVector.h"
#include "Physics/MyParticle.h"
#include "Physics/PhysicsWorld.h"
#include "RenderParticle.h"
#include <random> 

float scale = 20.0f;
float x_rot = 0.0f;

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {}

int main(void)
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    int width = 700;
    int height = 700;

    window = glfwCreateWindow(width, height, "Particle Spawn Test", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glEnable(GL_DEPTH_TEST);

    OrthoCamera camera(width, height);
    glViewport(0, 0, width, height);
    glfwSetKeyCallback(window, Key_Callback);

    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff; vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str(); const char* vert = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff; fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str(); const char* frag = fragS.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vert, NULL); glCompileShader(vertShader);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &frag, NULL); glCompileShader(fragShader);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader); glAttachShader(shaderProgram, fragShader); glLinkProgram(shaderProgram);

    // Model loading
    ModelLoader* sphereModel = new ModelLoader();
    GLuint vbo, ebo;
    if (!ModelLoader::LoadObjAndSetupBuffers("3D/sphere.obj", sphereModel->vao, vbo, ebo, sphereModel->indexCount)) {
        std::cerr << "Failed to load model!" << std::endl; return -1;
    }

    Physics::PhysicsWorld world;
    std::vector<Physics::Particle*> particles;
    std::vector<Physics::RenderParticle*> renderParticles;

    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now(), prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);
    const std::chrono::nanoseconds timestep(1600000);

    // For spawn timing
    float spawnInterval = 0.5f; 
    float spawnTimer = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        curr_time = clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
        prev_time = curr_time;
        curr_ns += dur;
        float dt = std::chrono::duration<float>(dur).count();
        spawnTimer += dt;

        if (particles.size() < 10 && spawnTimer >= spawnInterval) {
            spawnTimer = 0.0f;
            Physics::Particle* p = new Physics::Particle();
            // Bottom center of the screen
            p->Position = Physics::MyVector(0.0f, -height / 2.0f + 10.0f, 0.0f);
            p->mass = 1.0f;
            p->lifespan = 5.0f; // 5 seconds

            // Stronger random force: x and z in [-300, 300], y in [4000, 6000]
			float fx = static_cast<float>(rand()) / RAND_MAX * 20000.0f - 10000.0f;
            float fy = static_cast<float>(rand()) / RAND_MAX * 6000.0f + 12000.0f;
			float fz = static_cast<float>(rand()) / RAND_MAX * 20000.0f - 10000.0f;
            p->AddForce(Physics::MyVector(fx, fy, fz));

            world.AddParticle(p);
            auto* rp = new Physics::RenderParticle(p, sphereModel, Physics::MyVector(1, 0, 0));
            particles.push_back(p);
            renderParticles.push_back(rp);
        }


        //Physics Update
        if (curr_ns >= timestep) {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            curr_ns = std::chrono::nanoseconds(0);
            float dt = static_cast<float>(ms.count()) / 1000.0f;
            world.Update(dt);
        }

        // Remove dead particles 
        for (size_t i = 0; i < particles.size();) {
            if (particles[i]->IsDestroyed()) {
                delete renderParticles[i];
                delete particles[i];
                renderParticles.erase(renderParticles.begin() + i);
                particles.erase(particles.begin() + i);
            } else {
                ++i;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.GetProjection()));
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.GetView()));

        for (auto* rp : renderParticles) {
            rp->Draw(shaderProgram, scale, x_rot);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (Physics::RenderParticle* rp : renderParticles) delete rp;
    for (Physics::Particle* p : particles) delete p;
    delete sphereModel;
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glfwTerminate();
    return 0;
}
