#include <algorithm>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <UpstreamGL/Components/Camera.h>
#include <UpstreamGL/Components/Transform.h>
#include <UpstreamGL/Graphics/MeshData.h>
#include <UpstreamGL/Graphics/Renderer.h>

namespace upgl {
void Renderer::Render(Camera *camera) {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glFrontFace(GL_CW);

  auto vp = camera->GetProjectionMatrix() * camera->GetViewMatrix();

  for (auto &rg : m_RenderGroups) {
    auto s = rg.first;
    s.Bind();

    for (auto m : rg.second) {
      auto transform = m->GetNode()->GetComponent<Transform>();
      if (camera->IsFrustumCulling() &&
          !camera->IsInFrustum(transform->GetWorldPosition(), m->GetBounds().GetSize()))
        continue;

      m->Render(vp);
    }
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (auto &rg : m_RenderGroupsAlpha) {
    auto s = rg.first;
    s.Bind();

    for (auto m : rg.second) {
      auto transform = m->GetNode()->GetComponent<Transform>();
      if (camera->IsFrustumCulling() &&
          !camera->IsInFrustum(transform->GetWorldPosition(), m->GetBounds().GetSize()))
        continue;

      m->Render(vp);
    }
  }

  glDisable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
}
} // namespace upgl