#include <reactphysics3d/reactphysics3d.h>

#include <UpstreamGL/Scene/Director.h>
#include <UpstreamGL/Scene/Scene.h>

namespace upgl {
Director *Director::GetInstance() {
  static Director __director;
  return &__director;
}

Renderer &Director::GetMeshRenderer() { return m_MeshRenderer; }

Renderer &Director::GetUIRenderer() { return m_UIRenderer; }

void Director::Tick(float deltaTime) { m_Scene.Tick(deltaTime); }

void Director::FixedUpdate(float fixedDeltaTime) {
  m_Scene.GetPhysicsWorld().update(fixedDeltaTime);
}
} // namespace upgl