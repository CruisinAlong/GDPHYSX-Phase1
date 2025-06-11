#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <glad/glad.h>
#include "tiny_obj_loader.h"
#include <iostream>



class ModelLoader
{
public:
    GLuint vao = 0;
    size_t indexCount = 0;
    glm::vec3 Position;
    glm::vec3 Color;

    // Loads the OBJ file, creates VAO/VBO/EBO, and returns the number of indices
    static bool LoadObjAndSetupBuffers(
        const std::string& path,
        GLuint& vao,
        GLuint& vbo,
        GLuint& ebo,
        size_t& indexCount)
    {
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warning, error;
        tinyobj::attrib_t attributes;

        bool success = tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, path.c_str());
        if (!success || shapes.empty())
        {
            std::cerr << "Failed to load OBJ: " << error << std::endl;
            return false;
        }

        // Prepare index buffer
        std::vector<GLuint> mesh_indices;
        for (const auto& idx : shapes[0].mesh.indices)
            mesh_indices.push_back(idx.vertex_index);

        // Create buffers
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * attributes.vertices.size(), attributes.vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), mesh_indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        indexCount = mesh_indices.size();
        return true;
    }

    void Draw(GLuint shaderProgram, float scale, float x_rot)
    {
        // Debug log
        /*std::cout << "[ModelLoader::Draw] Position: ("
            << Position.x << ", " << Position.y << ", " << Position.z << ") "
            << "Color: (" << Color.r << ", " << Color.g << ", " << Color.b << ") "
            << "Scale: " << scale << " x_rot: " << x_rot << std::endl;*/

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, Position);
        transform = glm::rotate(transform, glm::radians(x_rot), glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::scale(transform, glm::vec3(scale, scale, scale));
        if (shaderProgram) {
            unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");
            glUniform3fv(colorLoc, 1, &Color[0]);
        }
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

};
