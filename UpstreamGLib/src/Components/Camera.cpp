#include "Camera.h"
#include "System/Application.h"
#include "System/Window.h"
#include "Scene/Node.h"
#include "System/Scheduler.h"

Camera::Camera(Node* node, CameraSettings settings)
    : Component(node), m_CameraSettings(settings) 
{
    Application::GetInstance()->GetWindow()->OnWindowResize() += 
        EventDelegate<WindowResizeEvent>{ ConnectFunc<&Camera::__WindowResizeCallback>, this };

    int width = 0, height = 0;
    Application::GetInstance()->GetWindow()->GetSize(width, height);

    Application::GetInstance()->GetScheduler()->Register(UpdateDelegate{ConnectFunc<&Camera::__Update>, this});

    m_ProjectionMatrix = 
        glm::perspective(glm::radians(m_CameraSettings.FieldOfView), (float)width / (float)height, m_CameraSettings.NearClip, m_CameraSettings.FarClip);
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
    m_ViewMatrix = glm::lookAt(m_Node->GetWorldPosition(), m_Node->GetWorldPosition() + m_Node->GetForwardDirection(), m_Node->GetUpDirection());
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

bool Camera::__WindowResizeCallback(const WindowResizeEvent* e)
{
    m_ProjectionMatrix = 
        glm::perspective(glm::radians(m_CameraSettings.FieldOfView), 
        (float)e->Width / (float)e->Height, m_CameraSettings.NearClip, m_CameraSettings.FarClip);
    return true;
}

void Camera::__Update(float deltaTime)
{
    CalculateViewMatrix();
    CalculateFrustumPlanes();
}