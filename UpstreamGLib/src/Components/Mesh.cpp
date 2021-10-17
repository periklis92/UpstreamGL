#include <UpstreamGL/Components/Mesh.h>
#include <UpstreamGL/Scene/Director.h>
#include <UpstreamGL/Components/Camera.h>

void Mesh::OnEnter()
{
    Director::GetInstance()->GetMeshRenderer().AddToQueue(this);
}

void Mesh::Render(Camera* camera)
{
    auto transform = m_Node->GetComponent<Transform>();
    if (!m_MeshResource || !m_Shader ||
        !camera->IsInFrustum(transform->GetWorldPosition(), m_MeshResource->GetBounds().GetSize()))
            return;

    m_Shader->Bind();
    if (m_MeshResource->IsSkinned())
    {
        auto bTransforms = m_MeshResource->GetArmature()->GetBoneTransforms();
        m_Shader->SetMat4(GLR_SHADER_BONE_TRANSFORM_NAME, bTransforms.size(), bTransforms.data());
    }
    m_Shader->SetMat4(GLR_SHADER_MVP_NAME, camera->GetProjectionMatrix() * camera->GetViewMatrix() * transform->GetWorldTransform());
    m_MeshResource->Draw();
}

void Mesh::UpdateFromAnimation(const AnimationClip& clip, float time)
{
    m_MeshResource->GetArmature()->UpdateFromAnimation(clip, time);
}