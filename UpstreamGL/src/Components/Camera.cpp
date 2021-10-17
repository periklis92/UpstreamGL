#include <UpstreamGL/Components/Camera.h>
#include <UpstreamGL/System/Application.h>
#include <UpstreamGL/System/EventDispatcher.h>
#include <UpstreamGL/System/Scheduler.h>
#include <UpstreamGL/System/Window.h>

Camera::Camera(Node* node)
    : Component(node)
{
    Application::GetInstance()->GetWindow()->OnWindowResize() += 
        EventDelegate<WindowResizeEvent>{ ConnectFunc<&Camera::__WindowResizeCallback>, this };
    Application::GetInstance()->GetWindow()->GetSize(m_ScreenWidth, m_ScreenHeight);
    Application::GetInstance()->GetScheduler()->RegisterUpdate(UpdateDelegate{ConnectFunc<&Camera::__Update>, this});
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return m_ProjectionMatrix;
}

glm::mat4 Camera::GetViewMatrix() const
{    
    return m_ViewMatrix;
}

void Camera::CalculateViewMatrix()
{
    auto transform = m_Node->GetComponent<Transform>();
    m_ViewMatrix = glm::lookAt(transform->GetWorldPosition(), transform->GetWorldPosition() + transform->GetForwardDirection(), transform->GetUpDirection());
}

void Camera::CalculateFrustumPlanes()
{
        glm::mat4 vp = GetProjectionMatrix() * GetViewMatrix();

        m_FrustumPlanes[0] = glm::vec4( vp[0][3] - vp[0][0],
                                        vp[1][3] - vp[1][0],
                                        vp[2][3] - vp[2][0],
                                        vp[3][3] - vp[3][0]);
                //Left
        m_FrustumPlanes[1] = glm::vec4( vp[0][3] + vp[0][0],
                                        vp[1][3] + vp[1][0],
                                        vp[2][3] + vp[2][0],
                                        vp[3][3] + vp[3][0]);
        //Bottom
        m_FrustumPlanes[2] = glm::vec4( vp[0][3] + vp[0][1],
                                        vp[1][3] + vp[1][1],
                                        vp[2][3] + vp[2][1],
                                        vp[3][3] + vp[3][1]);
        //Top
        m_FrustumPlanes[3] = glm::vec4( vp[0][3] - vp[0][1],
                                        vp[1][3] - vp[1][1],
                                        vp[2][3] - vp[2][1],
                                        vp[3][3] - vp[3][1]);
        //Far
        m_FrustumPlanes[4] = glm::vec4( vp[0][3] - vp[0][2],
                                        vp[1][3] - vp[1][2],
                                        vp[2][3] - vp[2][2],
                                        vp[3][3] - vp[3][2]);
        //Near
        m_FrustumPlanes[5] = glm::vec4( vp[0][3] + vp[0][2],
                                        vp[1][3] + vp[1][2],
                                        vp[2][3] + vp[2][2],
                                        vp[3][3] + vp[3][2]);

        for (int i = 0; i < 6; i++)
        {
                m_FrustumPlanes[i] = glm::normalize(m_FrustumPlanes[i]);
                float invl = glm::sqrt(m_FrustumPlanes[i].x * m_FrustumPlanes[i].x +
                        m_FrustumPlanes[i].y * m_FrustumPlanes[i].y +
                        m_FrustumPlanes[i].z * m_FrustumPlanes[i].z);
                m_FrustumPlanes[i] /= invl;
        }
}

bool Camera::IsInFrustum(glm::vec3 pos, glm::vec3 size) const
{    
    for (int i = 0; i < 6; ++i)
    {
        float 
        dist =  m_FrustumPlanes[i].x * (pos.x - size.x) +
                m_FrustumPlanes[i].y * (pos.y - size.y) +
                m_FrustumPlanes[i].z * (pos.z - size.z) +
                m_FrustumPlanes[i].w;
        if (dist > 0) continue;

        dist = m_FrustumPlanes[i].x * (pos.x + size.x) +
               m_FrustumPlanes[i].y * (pos.y - size.y) +
               m_FrustumPlanes[i].z * (pos.z - size.z) +
               m_FrustumPlanes[i].w;
        if (dist > 0) continue;

        dist =  m_FrustumPlanes[i].x * (pos.x - size.x) +
                m_FrustumPlanes[i].y * (pos.y + size.y) +
                m_FrustumPlanes[i].z * (pos.z - size.z) +
                m_FrustumPlanes[i].w;
        if (dist > 0) continue;

        dist =  m_FrustumPlanes[i].x * (pos.x - size.x) +
                m_FrustumPlanes[i].y * (pos.y - size.y) +
                m_FrustumPlanes[i].z * (pos.z + size.z) +
                m_FrustumPlanes[i].w;
        if (dist > 0) continue;

        dist =  m_FrustumPlanes[i].x * (pos.x + size.x) +
                m_FrustumPlanes[i].y * (pos.y - size.y) +
                m_FrustumPlanes[i].z * (pos.z + size.z) +
                m_FrustumPlanes[i].w;
        if (dist > 0) continue;

        dist =  m_FrustumPlanes[i].x * (pos.x - size.x) +
                m_FrustumPlanes[i].y * (pos.y + size.y) +
                m_FrustumPlanes[i].z * (pos.z + size.z) +
                m_FrustumPlanes[i].w;
        if (dist > 0) continue;

        dist =  m_FrustumPlanes[i].x * (pos.x + size.x) +
                m_FrustumPlanes[i].y * (pos.y + size.y) +
                m_FrustumPlanes[i].z * (pos.z + size.z) +
                m_FrustumPlanes[i].w;
        if (dist > 0) continue;

        return false;
    }
    return true;
}

glm::vec3 Camera::ScreenToWorldSpace(const glm::vec2& screenPosition)
{
    glm::mat4 inv = glm::inverse(m_ProjectionMatrix * m_ViewMatrix);
    glm::vec4 pos(  (screenPosition.x / (float)m_ScreenWidth) , 
                    (screenPosition.y / (float)m_ScreenHeight) , 
                    m_Node->GetTransform()->GetWorldPosition().z, 
                    1.f);
                    
    pos = pos * 2.0f - 1.0f;                        
    pos = inv * pos;
    pos /= pos.w;
    return pos;
}

bool Camera::__WindowResizeCallback(const WindowResizeEvent* e)
{
    m_ScreenWidth = e->Width;
    m_ScreenHeight = e->Height;
    return true;
}

void Camera::__Update(float deltaTime)
{
    CalculateViewMatrix();
    CalculateFrustumPlanes();
}

PerspectiveCamera::PerspectiveCamera(Node* node, PerspectiveCameraSettings settings)
    :Camera(node), m_CameraSettings(settings)
{
    m_ProjectionMatrix = 
        glm::perspective(glm::radians(m_CameraSettings.FieldOfView), (float)m_ScreenWidth / (float)m_ScreenHeight, m_CameraSettings.NearClip, m_CameraSettings.FarClip);
}

bool PerspectiveCamera::__WindowResizeCallback(const WindowResizeEvent* e)
{
    Camera::__WindowResizeCallback(e);
    m_ProjectionMatrix = 
        glm::perspective(glm::radians(m_CameraSettings.FieldOfView), 
        (float)e->Width / (float)e->Height, m_CameraSettings.NearClip, m_CameraSettings.FarClip);
    return true;
}


OrthoCamera::OrthoCamera(Node* node, OrthoCameraSettings settings)
    :Camera(node), m_CameraSettings(settings)
{

}

bool OrthoCamera::__WindowResizeCallback(const WindowResizeEvent* e)
{
    Camera::__WindowResizeCallback(e);
    float aspectRatio = e->Width / e->Height;
    float height = m_CameraSettings.size * aspectRatio;
    m_ProjectionMatrix = 
        glm::ortho(-m_CameraSettings.size / 2.f, m_CameraSettings.size / 2.f, height / 2.f,  -height / 2.f);
    return true;
}