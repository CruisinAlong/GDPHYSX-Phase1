#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class OrthoCamera
{
public:
    OrthoCamera(float width, float height, float nearPlane = -100.0f, float farPlane = 100.0f)
    {
        SetProjection(width, height, nearPlane, farPlane);
        view = glm::mat4(1.0f);
    }

    void SetProjection(float width, float height, float nearPlane = -800.0f, float farPlane = 800.0f)
    {
        float orthoHalfWidth = width;
        float orthoHalfHeight = height;
        projection = glm::ortho(
            -orthoHalfWidth, orthoHalfWidth,
            -orthoHalfHeight, orthoHalfHeight,
            nearPlane, farPlane
        );
    }

    const glm::mat4& GetProjection() const { return projection; }
    const glm::mat4& GetView() const { return view; }
    void SetView(const glm::mat4& v) { view = v; }

private:
    glm::mat4 projection;
    glm::mat4 view;
};
