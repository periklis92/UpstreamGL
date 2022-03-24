#include <UpstreamGL/Components/Camera.h>
#include <UpstreamGL/Components/Transform.h>
#include <UpstreamGL/System/Application.h>
#include <UpstreamGL/System/EventDispatcher.h>
#include <UpstreamGL/System/Scheduler.h>
#include <UpstreamGL/System/Window.h>

namespace upgl {
Camera::Camera(Node *node) : Component(node) {
  Application::GetInstance()->GetWindow()->OnWindowResize() +=
      EventDelegate<WindowResizeEvent>{ConnectFunc<&Camera::__WindowResizeCallback>, this};
  Application::GetInstance()->GetWindow()->GetSize(m_ScreenWidth, m_ScreenHeight);
  Application::GetInstance()->GetScheduler()->RegisterUpdate(
      UpdateDelegate{ConnectFunc<&Camera::__Update>, this});
}

glm::mat4 Camera::GetProjectionMatrix() const { return m_ProjectionMatrix; }

glm::mat4 Camera::GetViewMatrix() const { return m_ViewMatrix; }

void Camera::CalculateViewMatrix() {
  auto transform = m_Node->GetComponent<Transform>();
  m_ViewMatrix = glm::lookAt(transform->GetWorldPosition(),
                             transform->GetWorldPosition() + transform->GetForwardDirection(),
                             transform->GetUpDirection());
}

glm::vec3 Camera::ScreenPointToWorldSpace(const glm::vec2 &screenPosition) const {
  return glm::unProject(glm::vec3(screenPosition.x, m_ScreenHeight - screenPosition.y,
                                  m_Node->GetTransform()->GetWorldPosition().z),
                        m_ViewMatrix, m_ProjectionMatrix,
                        glm::vec4{0, 0, m_ScreenWidth, m_ScreenHeight});
}

Ray Camera::ScreenPointToRay(const glm::vec2 &screenPosition) const {
  auto transform = m_Node->GetTransform();
  auto start = glm::unProject(glm::vec3(screenPosition.x, m_ScreenHeight - screenPosition.y, 0.0f),
                              m_ViewMatrix, m_ProjectionMatrix,
                              glm::vec4{0, 0, m_ScreenWidth, m_ScreenHeight});
  auto end = glm::unProject(glm::vec3(screenPosition.x, m_ScreenHeight - screenPosition.y, 1.0f),
                            m_ViewMatrix, m_ProjectionMatrix,
                            glm::vec4{0, 0, m_ScreenWidth, m_ScreenHeight});
  auto dir = glm::normalize(end);
  return Ray{start, end};
}

void Camera::CalculateFrustumPlanes() {
  glm::mat4 vp = GetProjectionMatrix() * GetViewMatrix();

  // Right
  m_FrustumPlanes[0] =
      glm::vec4(vp[0][3] - vp[0][0], vp[1][3] - vp[1][0], vp[2][3] - vp[2][0], vp[3][3] - vp[3][0]);
  // Left
  m_FrustumPlanes[1] =
      glm::vec4(vp[0][3] + vp[0][0], vp[1][3] + vp[1][0], vp[2][3] + vp[2][0], vp[3][3] + vp[3][0]);
  // Bottom
  m_FrustumPlanes[2] =
      glm::vec4(vp[0][3] + vp[0][1], vp[1][3] + vp[1][1], vp[2][3] + vp[2][1], vp[3][3] + vp[3][1]);
  // Top
  m_FrustumPlanes[3] =
      glm::vec4(vp[0][3] - vp[0][1], vp[1][3] - vp[1][1], vp[2][3] - vp[2][1], vp[3][3] - vp[3][1]);
  // Far
  m_FrustumPlanes[4] =
      glm::vec4(vp[0][3] - vp[0][2], vp[1][3] - vp[1][2], vp[2][3] - vp[2][2], vp[3][3] - vp[3][2]);
  // Near
  m_FrustumPlanes[5] =
      glm::vec4(vp[0][3] + vp[0][2], vp[1][3] + vp[1][2], vp[2][3] + vp[2][2], vp[3][3] + vp[3][2]);

  for (int i = 0; i < 6; i++) {
    m_FrustumPlanes[i] = glm::normalize(m_FrustumPlanes[i]);
    float invl = glm::sqrt(m_FrustumPlanes[i].x * m_FrustumPlanes[i].x +
                           m_FrustumPlanes[i].y * m_FrustumPlanes[i].y +
                           m_FrustumPlanes[i].z * m_FrustumPlanes[i].z);
    m_FrustumPlanes[i] /= invl;
  }
}

bool Camera::IsInFrustum(glm::vec3 pos, glm::vec3 size) const {
  for (int i = 0; i < 6; ++i) {
    float dist = m_FrustumPlanes[i].x * (pos.x - size.x) + m_FrustumPlanes[i].y * (pos.y - size.y) +
                 m_FrustumPlanes[i].z * (pos.z - size.z) + m_FrustumPlanes[i].w;
    if (dist > 0)
      continue;

    dist = m_FrustumPlanes[i].x * (pos.x + size.x) + m_FrustumPlanes[i].y * (pos.y - size.y) +
           m_FrustumPlanes[i].z * (pos.z - size.z) + m_FrustumPlanes[i].w;
    if (dist > 0)
      continue;

    dist = m_FrustumPlanes[i].x * (pos.x - size.x) + m_FrustumPlanes[i].y * (pos.y + size.y) +
           m_FrustumPlanes[i].z * (pos.z - size.z) + m_FrustumPlanes[i].w;
    if (dist > 0)
      continue;

    dist = m_FrustumPlanes[i].x * (pos.x - size.x) + m_FrustumPlanes[i].y * (pos.y - size.y) +
           m_FrustumPlanes[i].z * (pos.z + size.z) + m_FrustumPlanes[i].w;
    if (dist > 0)
      continue;

    dist = m_FrustumPlanes[i].x * (pos.x + size.x) + m_FrustumPlanes[i].y * (pos.y - size.y) +
           m_FrustumPlanes[i].z * (pos.z + size.z) + m_FrustumPlanes[i].w;
    if (dist > 0)
      continue;

    dist = m_FrustumPlanes[i].x * (pos.x - size.x) + m_FrustumPlanes[i].y * (pos.y + size.y) +
           m_FrustumPlanes[i].z * (pos.z + size.z) + m_FrustumPlanes[i].w;
    if (dist > 0)
      continue;

    dist = m_FrustumPlanes[i].x * (pos.x + size.x) + m_FrustumPlanes[i].y * (pos.y + size.y) +
           m_FrustumPlanes[i].z * (pos.z + size.z) + m_FrustumPlanes[i].w;
    if (dist > 0)
      continue;

    return false;
  }
  return true;
}

bool Camera::__WindowResizeCallback(const WindowResizeEvent *e) {
  m_ScreenWidth = e->Width;
  m_ScreenHeight = e->Height;
  return true;
}

void Camera::__Update(float deltaTime) {
  CalculateViewMatrix();
  CalculateFrustumPlanes();
}

PerspectiveCamera::PerspectiveCamera(Node *node, PerspectiveCameraSettings settings)
    : Camera(node), m_CameraSettings(settings) {
  m_ProjectionMatrix = glm::perspective(glm::radians(m_CameraSettings.FieldOfView),
                                        (float)m_ScreenWidth / (float)m_ScreenHeight,
                                        m_CameraSettings.NearClip, m_CameraSettings.FarClip);
}

bool PerspectiveCamera::__WindowResizeCallback(const WindowResizeEvent *e) {
  Camera::__WindowResizeCallback(e);
  m_ProjectionMatrix = glm::perspective(glm::radians(m_CameraSettings.FieldOfView),
                                        (float)m_ScreenWidth / (float)m_ScreenHeight,
                                        m_CameraSettings.NearClip, m_CameraSettings.FarClip);
  return true;
}

OrthoCamera::OrthoCamera(Node *node, OrthoCameraSettings settings)
    : Camera(node), m_CameraSettings(settings) {
  float aspectRatio = (float)m_ScreenWidth / (float)m_ScreenHeight;
  float height = m_CameraSettings.Size * aspectRatio;
  m_ProjectionMatrix =
      glm::orthoLH(-m_CameraSettings.Size / 2.f, m_CameraSettings.Size / 2.f, height / 2.f,
                   -height / 2.f, m_CameraSettings.NearClip, m_CameraSettings.FarClip);
}

bool OrthoCamera::__WindowResizeCallback(const WindowResizeEvent *e) {
  Camera::__WindowResizeCallback(e);
  float aspectRatio = (float)m_ScreenWidth / (float)m_ScreenHeight;
  float height = m_CameraSettings.Size * aspectRatio;
  m_ProjectionMatrix =
      glm::orthoLH(-m_CameraSettings.Size / 2.f, m_CameraSettings.Size / 2.f, height / 2.f,
                   -height / 2.f, m_CameraSettings.NearClip, m_CameraSettings.FarClip);
  return true;
}

UICamera::UICamera(Node *node) : Camera(node) {
  m_ProjectionMatrix = glm::ortho(0.f, (float)m_ScreenWidth, 0.f, (float)m_ScreenHeight);
}

bool UICamera::__WindowResizeCallback(const WindowResizeEvent *e) {
  Camera::__WindowResizeCallback(e);
  m_ProjectionMatrix = glm::ortho(0.f, (float)m_ScreenWidth, 0.f, (float)m_ScreenHeight);
  return true;
}
} // namespace upgl