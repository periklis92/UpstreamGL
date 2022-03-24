#pragma once

#include <UpstreamGL/Components/Component.h>
#include <UpstreamGL/Math/AABB.h>
#include <UpstreamGL/Physics/Raycast.h>
#include <UpstreamGL/Resources/MeshResource.h>
#include <UpstreamGL/Scene/Director.h>

#include <glm/glm.hpp>

namespace upgl {
struct WindowResizeEvent;

struct PerspectiveCameraSettings {
  float NearClip{.01f}, FarClip{1000.f};
  float FieldOfView{60.f};
};

struct OrthoCameraSettings {
  float Size{10};
  float NearClip{.01f}, FarClip{1000.f};
};

class Camera : public Component {
public:
  Camera(Node *node);

  glm::mat4 GetProjectionMatrix() const;
  glm::mat4 GetViewMatrix() const;

  virtual void OnEnter() override { Director::GetInstance()->GetScene()->SetMainCamera(this); }
  virtual void OnExit() override {}

  void SetFrustumCulling(bool enabled) { m_IsFrustumCulling = enabled; }
  bool IsFrustumCulling() { return m_IsFrustumCulling; }

  void CalculateViewMatrix();
  void CalculateFrustumPlanes();
  bool IsInFrustum(glm::vec3 pos, glm::vec3 size) const;

  virtual glm::vec3 ScreenPointToWorldSpace(const glm::vec2 &screenPosition) const;
  virtual Ray ScreenPointToRay(const glm::vec2 &screenPosition) const;

protected:
  virtual bool __WindowResizeCallback(const WindowResizeEvent *e);
  virtual void __Update(float deltaTime);

  friend class Scene;

protected:
  glm::vec4 m_FrustumPlanes[6];
  glm::mat4 m_ViewMatrix{0.f};
  glm::mat4 m_ProjectionMatrix{0.f};
  bool m_IsFrustumCulling = true;
  int m_ScreenWidth{0}, m_ScreenHeight{0};
};

class PerspectiveCamera : public Camera {
public:
  PerspectiveCamera(Node *node, PerspectiveCameraSettings settings = {});
  virtual const std::string GetComponentName() const override { return "PerspectiveCamera"; }

protected:
  virtual bool __WindowResizeCallback(const WindowResizeEvent *e) override;

private:
  PerspectiveCameraSettings m_CameraSettings;
};

class OrthoCamera : public Camera {
public:
  OrthoCamera(Node *node, OrthoCameraSettings settings = {});
  virtual const std::string GetComponentName() const override { return "OrthographicCamera"; }
  inline void SetCameraSettings(OrthoCameraSettings settings) { m_CameraSettings = settings; }

protected:
  virtual bool __WindowResizeCallback(const WindowResizeEvent *e) override;

private:
  OrthoCameraSettings m_CameraSettings;
};

class UICamera : public Camera {
public:
  UICamera(Node *node);
  virtual const std::string GetComponentName() const override { return "UICamera"; }

protected:
  virtual bool __WindowResizeCallback(const WindowResizeEvent *e) override;
};
} // namespace upgl