// PerspectiveCamera.h
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class PerspectiveCamera
{
public:
    PerspectiveCamera(float fovDegrees, float aspectRatio, float nearPlane = 0.1f, float farPlane = 5000.0f)
    {
        SetProjection(fovDegrees, aspectRatio, nearPlane, farPlane);
        view = glm::mat4(1.0f);
    }

    void SetProjection(float fovDegrees, float aspectRatio, float nearPlane = 0.1f, float farPlane = 5000.0f)
    {
        projection = glm::perspective(glm::radians(fovDegrees), aspectRatio, nearPlane, farPlane);
    }

    const glm::mat4& GetProjection() const { return projection; }
    const glm::mat4& GetView() const { return view; }
    void SetView(const glm::mat4& v) { view = v; }

private:
    glm::mat4 projection;
    glm::mat4 view;
};
