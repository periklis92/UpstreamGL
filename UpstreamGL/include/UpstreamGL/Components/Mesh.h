#pragma once

#include <UpstreamGL/Components/Component.h>
#include <UpstreamGL/Graphics/MeshObject.h>
#include <UpstreamGL/Graphics/Renderable.h>
#include <UpstreamGL/Resources/MeshResource.h>

namespace upgl {
class MeshData;
class Shader;
class Camera;
class Node;
class AnimationClip;

class Mesh final : public Component, public Renderable {
public:
  Mesh(Node *node) : Component(node) {}

  virtual void Render(glm::mat4 viewProj) override;

  virtual void OnEnter() override;
  virtual void OnExit() override;

  Node *GetNode() override { return Component::GetNode(); }

  void SetMesh(MeshResource mesh) {
    m_MeshObject = mesh.GetMeshObject();
    m_AABB = mesh->GetBounds();
  }
  AABB GetBounds() const override { return m_AABB; }
  IndexedMeshObject &GetMeshObject() { return m_MeshObject; }

  virtual const std::string GetComponentName() const override { return "Mesh"; }

private:
  IndexedMeshObject m_MeshObject;
  AABB m_AABB;
};

class SkinnedMesh final : public Component, public Renderable {
public:
  SkinnedMesh(Node *node) : Component(node) {}

  virtual void Render(glm::mat4 viewProj) override;
  void UpdateFromAnimation(const AnimationClip &clip, float time);

  virtual void OnEnter() override;
  virtual void OnExit() override;

  Node *GetNode() override { return Component::GetNode(); }

  void SetMesh(SkinnedMeshResource mesh) {
    m_MeshObject = mesh.GetMeshObject();
    m_AABB = mesh->GetBounds();
    m_Armature = mesh.GetArmature();
  }

  AABB GetBounds() const override { return m_AABB; }
  IndexedMeshObject &GetMeshObject() { return m_MeshObject; }

  virtual const std::string GetComponentName() const override { return "SkinnedMesh"; }

private:
  IndexedMeshObject m_MeshObject;
  Armature *m_Armature;
  AABB m_AABB;
};
} // namespace upgl