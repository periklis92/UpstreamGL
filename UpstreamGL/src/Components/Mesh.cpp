#include <UpstreamGL/Components/Camera.h>
#include <UpstreamGL/Components/Mesh.h>
#include <UpstreamGL/Components/Transform.h>
#include <UpstreamGL/Scene/Director.h>

namespace upgl {
void Mesh::OnEnter() { Director::GetInstance()->GetMeshRenderer().AddToQueue(this); }

void Mesh::OnExit() { Director::GetInstance()->GetMeshRenderer().RemoveFromQueue(this); }

void Mesh::Render(glm::mat4 viewProj) {
  m_Material->GetShader().SetMat4(UPGL_SHADER_MVP_NAME,
                                  viewProj * m_Node->GetTransform()->GetWorldTransform());
  m_Material->Apply();
  m_MeshObject.Bind();
  m_MeshObject.Render();
}

void SkinnedMesh::OnEnter() { Director::GetInstance()->GetMeshRenderer().AddToQueue(this); }

void SkinnedMesh::OnExit() { Director::GetInstance()->GetMeshRenderer().RemoveFromQueue(this); }

void SkinnedMesh::Render(glm::mat4 viewProj) {
  auto bTransforms = m_Armature->GetBoneTransforms();
  m_Material->GetShader().SetMat4(UPGL_SHADER_BONE_TRANSFORM_NAME, bTransforms.data(),
                                  bTransforms.size());
  m_Material->GetShader().SetMat4(UPGL_SHADER_MVP_NAME,
                                  viewProj * m_Node->GetTransform()->GetWorldTransform());
  m_Material->Apply();
  m_MeshObject.Bind();
  m_MeshObject.Render();
}

void SkinnedMesh::UpdateFromAnimation(const AnimationClip &clip, float time) {
  m_Armature->UpdateFromAnimation(clip, time);
}
} // namespace upgl