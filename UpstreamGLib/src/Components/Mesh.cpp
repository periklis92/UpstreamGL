#include "Mesh.h"

#include "Camera.h"
#include "Debug/Logger.h"
#include "Resources/MeshResource.h"
#include "Graphics/MeshData.h"
#include "Graphics/Shader.h"
#include "Scene/Node.h"
#include "Scene/Director.h"
#include "Graphics/MeshRenderer.h"

void Mesh::OnEnter()
{
    Director::GetInstance()->GetMeshRenderer().AddToQueue(this);
}

void Mesh::Render(Camera* camera)
{
    if (!m_MeshData || !m_Shader ||
        !camera->IsInFrustum(m_Node->GetWorldPosition(), m_MeshData->GetAABB().GetSize() / 2.f))
            return;

    m_Shader->Bind();
    if (m_MeshData->IsSkinned())
    {
        auto bTransforms = m_MeshData->GetArmature()->GetBoneTransforms();
        m_Shader->SetMat4(GLR_SHADER_BONE_TRANSFORM_NAME, bTransforms.size(), bTransforms.data());
    }
    m_Shader->SetMat4(GLR_SHADER_MVP_NAME, camera->GetProjectionMatrix() * camera->GetViewMatrix() * m_Node->GetWorldTransform());
    m_MeshData->Draw();
}

void Mesh::UpdateFromAnimation(const AnimationClip& clip, float time)
{
    m_MeshData->GetArmature()->UpdateFromAnimation(clip, time);
}