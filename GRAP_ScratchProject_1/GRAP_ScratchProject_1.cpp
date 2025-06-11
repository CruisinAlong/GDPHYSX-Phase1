#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include "OrthoCamera.h"
#include "PerspectiveCamera.h"
#include "ModelLoader.h"
#include "Physics/MyVector.h"
#include "Physics/MyParticle.h"
#include "Physics/PhysicsWorld.h"
#include "RenderParticle.h"
#include <random> 

using namespace std::chrono_literals;

constexpr std::chrono::nanoseconds timeStep(16ms);

float scale = 20.0f;
float x_rot = 0.0f;

// Values for camera position and rotation
float x_cam = 0;
float y_cam = 0;
float z_cam = 0;

float yaw = 270;
float pitch = 0;

float camSpeed = 1.0f;
float camLookSpeed = 0.035f;

std::unordered_map<int, bool> keyStates;

bool usePerspective = true;
bool changeCamPressed = false;
PerspectiveCamera* perspectiveCamera = nullptr;
OrthoCamera* orthoCamera = nullptr;

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keyStates[key] = true;
        if (key == GLFW_KEY_E && !changeCamPressed) {
            usePerspective = !usePerspective;
            changeCamPressed = true;
        }
    }
    else if (action == GLFW_RELEASE) {
        keyStates[key] = false;
        if (key == GLFW_KEY_E) {
            changeCamPressed = false;
        }
    }
}


// Set a fixed radius for the orbit
float camRadius = 1000.0f;

void ProcessInput() {
    // Only update pitch/yaw for orbiting
    if (keyStates[GLFW_KEY_W]) pitch += camLookSpeed;
    if (keyStates[GLFW_KEY_S]) pitch -= camLookSpeed;
    if (keyStates[GLFW_KEY_A]) yaw += camLookSpeed;
    if (keyStates[GLFW_KEY_D]) yaw -= camLookSpeed;

    // Clamp pitch to avoid flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

}


int main(void)
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    int width = 800;
    int height = 800;

    window = glfwCreateWindow(width, height, "Particle Spawn Test", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glEnable(GL_DEPTH_TEST);

    float fov = 60.0f;
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    float nearPlane = 0.1f;
    float farPlane = 5000.0f;

    perspectiveCamera = new PerspectiveCamera(fov, aspect, nearPlane, farPlane);
    orthoCamera = new OrthoCamera(width, height);


    x_cam = 0.0f;
    y_cam = 0.0f;
    z_cam = 1000.0f;
    yaw = 270.0f;
    pitch = 0.0f;

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

    // For spawn timing
    float spawnInterval = 0.1f; 
    float spawnTimer = 0.0f;

	// Number of particles to spawn 
    int particleSpawnCount = 100;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        ProcessInput();

        // Calculate camera position based on spherical coordinates
        float yawRad = glm::radians(yaw);
        float pitchRad = glm::radians(pitch);

        float x = camRadius * cos(pitchRad) * cos(yawRad);
        float y = camRadius * sin(pitchRad);
        float z = camRadius * cos(pitchRad) * sin(yawRad);

        glm::vec3 camPos(x, y, z);
        glm::vec3 camTarget(0.0f, 0.0f, 0.0f); // Always look at the center
        glm::vec3 camUp(0.0f, 1.0f, 0.0f);

        glm::mat4 view;
        glm::mat4 projection;

        if (usePerspective) {
            // Update perspective camera view
            perspectiveCamera->SetView(glm::lookAt(camPos, camTarget, camUp));
            view = perspectiveCamera->GetView();
            projection = perspectiveCamera->GetProjection();
        }
        else {
            // Update ortho camera view
            orthoCamera->SetView(glm::lookAt(camPos, camTarget, camUp));
            view = orthoCamera->GetView();
            projection = orthoCamera->GetProjection();
        }


        curr_time = clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
        prev_time = curr_time;
        curr_ns += dur;
        float dt = std::chrono::duration<float>(dur).count();
        spawnTimer += dt;

        // Spawning block
        while (particles.size() <= particleSpawnCount && spawnTimer >= spawnInterval) {
            spawnTimer -= spawnInterval;
            Physics::Particle* p = new Physics::Particle();
            // Bottom center of the screen
            p->Position = Physics::MyVector(0.0f, -height / 2.0f + 10.0f, 0.0f);
            p->mass = 1.0f;
            p->lifespan = 3.0f; // 5 seconds

            // Stronger random force: x and z in [-300, 300], y in [4000, 6000]
            float fx = static_cast<float>(rand()) / RAND_MAX * 20000.0f - 10000.0f;
            float fy = static_cast<float>(rand()) / RAND_MAX * 6000.0f + 12000.0f;
            float fz = static_cast<float>(rand()) / RAND_MAX * 20000.0f - 10000.0f;
            p->AddForce(Physics::MyVector(fx, fy, fz));

            float r = colorDist(gen);
            float g = colorDist(gen);
            float b = colorDist(gen);

            world.AddParticle(p);
            auto* rp = new Physics::RenderParticle(p, sphereModel, Physics::MyVector(r, g, b));
            particles.push_back(p);
            renderParticles.push_back(rp);
        }



        //Physics Update
        if (curr_ns >= timeStep) {
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
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


        for (auto* rp : renderParticles) {
            rp->Draw(shaderProgram, scale, x_rot);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (Physics::RenderParticle* rp : renderParticles) delete rp;
    for (Physics::Particle* p : particles) delete p;
    delete sphereModel;
    delete perspectiveCamera;
    delete orthoCamera;

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glfwTerminate();
    return 0;
}
