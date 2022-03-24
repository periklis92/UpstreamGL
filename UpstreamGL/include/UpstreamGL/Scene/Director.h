#pragma once

#include <UpstreamGL/Graphics/Renderer.h>

#include "Scene.h"

namespace upgl {
class Director {
public:
  static Director *GetInstance();
  Renderer &GetMeshRenderer();
  Renderer &GetUIRenderer();
  Scene *const GetScene() { return &m_Scene; }
  void FixedUpdate(float fixedDeltaTime);

  void Tick(float deltaTime);

private:
  Director() = default;

  Renderer m_MeshRenderer{};
  Renderer m_UIRenderer{};
  Scene m_Scene{};
};
} // namespace upgl