#pragma once

#include <string>

#include <UpstreamGL/Physics/Raycast.h>
#include <UpstreamGL/System/BlockList.h>

#include "Node.h"

namespace reactphysics3d {
class PhysicsWorld;
class PhysicsCommon;
} // namespace reactphysics3d

namespace upgl {
class Camera;

using NodeContainer = BlockList<Node, 1024>;

class Scene {
public:
  Scene();

  void Unload();

  Node &CreateNode(const ::std::string &name = "");

  NodeContainer::iterator FindNode(const ::std::string &name) const;
  NodeContainer::iterator FindNode(const ID &id) const;

  void SetMainCamera(Camera *camera) { m_MainCamera = camera; }

  ::reactphysics3d::PhysicsWorld &GetPhysicsWorld() { return *m_PhysicsWorld; }
  ::reactphysics3d::PhysicsCommon &GetPhysicsCommon();
  Camera *GetMainCamera() const { return m_MainCamera; }
  Camera *GetUICamera() const { return m_UICamera; }

  bool Raycast(const Ray &ray, RaycastInfo &info);

  bool IsLoaded() const { return m_IsLoaded; }

  void Tick(float deltaTime);

  void Enter();
  void Exit();

private:
  NodeContainer m_Nodes{};
  Camera *m_MainCamera{nullptr};
  Camera *m_UICamera{nullptr};
  ::reactphysics3d::PhysicsWorld *m_PhysicsWorld{nullptr};
  bool m_IsLoaded{false};
};
} // namespace upgl