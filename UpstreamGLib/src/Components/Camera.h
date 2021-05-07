#pragma once

#include "Component.h"
#include "Math/AABB.h"
#include "glm/glm.hpp"
#include "Scene/Director.h"

struct WindowResizeEvent;

struct CameraSettings
{
    float NearClip{.01f}, FarClip{1000.f};
    float FieldOfView{60.f};
};

class Camera
    : public Component
{
public:
    Camera(Node* node, CameraSettings settings = {});

    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewMatrix() const;

    virtual void OnEnter() override { Director::GetInstance()->GetScene()->SetMainCamera(this); }
    virtual void OnExit() override {}

    void CalculateViewMatrix();
    void CalculateFrustumPlanes();
    bool IsInFrustum(glm::vec3 pos, glm::vec3 size) const;

    virtual const std::string GetComponentName() const override { return "Camera"; }

private:
    bool __WindowResizeCallback(const WindowResizeEvent* e);
    void __Update(float deltaTime);

private:
    CameraSettings m_CameraSettings;
    glm::vec4 m_FrustumPlanes[6];
    glm::mat4 m_ViewMatrix{0.f};
    glm::mat4 m_ProjectionMatrix{0.f};
};