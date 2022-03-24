#include <algorithm>

#include <reactphysics3d/reactphysics3d.h>

#include <UpstreamGL/Components/Camera.h>
#include <UpstreamGL/Components/Collider.h>
#include <UpstreamGL/Physics/Collision.h>
#include <UpstreamGL/Physics/Physics.h>
#include <UpstreamGL/Scene/Scene.h>
#include <UpstreamGL/System/ID.h>

namespace upgl {
Scene::Scene() { m_PhysicsWorld = Physics::CreatePhysicsWorld(); }

void Scene::Unload() {
  m_Nodes.clear();
  m_IsLoaded = false;
}

Node &Scene::CreateNode(const std::string &name) {
  auto &node = m_Nodes.emplace(Node(this, ID::Generate(), name));
  node.AddComponent<Transform>();
  return node;
}

bool Scene::Raycast(const Ray &ray, RaycastInfo &info) { return Physics::Raycast(ray, info, this); }

void Scene::Enter() {
  auto &camNode = CreateNode("__UICamera");
  m_UICamera = camNode.AddComponent<UICamera>();
  m_UICamera->SetFrustumCulling(false);
  for (auto &n : m_Nodes)
    n.OnEnter();
  m_IsLoaded = true;
}

void Scene::Tick(float deltaTime) {}

void Scene::Exit() {
  m_UICamera = nullptr;
  for (auto &n : m_Nodes)
    n.OnExit();
  Physics::GetPhysicsCommon().destroyPhysicsWorld(m_PhysicsWorld);
}

NodeContainer::iterator Scene::FindNode(const std::string &name) const {
  auto it = std::find_if(m_Nodes.begin(), m_Nodes.end(),
                         [&name](Node &n) { return n.GetName() == name; });
  return it;
}

NodeContainer::iterator Scene::FindNode(const ID &id) const {
  auto it =
      std::find_if(m_Nodes.begin(), m_Nodes.end(), [&id](Node &n) { return n.GetID() == id; });
  return it;
}

} // namespace upgl