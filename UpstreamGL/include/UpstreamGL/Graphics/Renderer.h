#pragma once

#include <list>
#include <map>

#include "Material.h"
#include "Renderable.h"
#include "Shader.h"
#include "Texture2D.h"

namespace upgl {
class Camera;

class Renderer {
  using RenderableCont = ::std::vector<Renderable *>;

public:
  void Render(Camera *camera);

  void AddToQueue(Renderable *mesh) {
    ::std::map<Material, RenderableCont>::iterator it;
    auto &mat = mesh->GetMateriaInstance().GetMaterial();
    if (!mat.HasAlpha()) {
      it = m_RenderGroups.find(mat);
      if (it == m_RenderGroups.end())
        it = m_RenderGroups.emplace(mat, RenderableCont{}).first;
    } else {
      it = m_RenderGroupsAlpha.find(mat);
      if (it == m_RenderGroupsAlpha.end())
        it = m_RenderGroupsAlpha.emplace(mat, RenderableCont{}).first;
    }
    it->second.push_back(mesh);
  }

  void RemoveFromQueue(Renderable *mesh) {
    ::std::map<Material, RenderableCont>::iterator it;
    RenderableCont::iterator iter;
    auto &mat = mesh->GetMateriaInstance().GetMaterial();
    if (!mat.HasAlpha()) {
      it = m_RenderGroups.find(mat);
      if (it == m_RenderGroups.end())
        return;
      iter = ::std::find(it->second.begin(), it->second.end(), mesh);
    } else {
      it = m_RenderGroupsAlpha.find(mat);
      if (it == m_RenderGroupsAlpha.end())
        return;
      iter = ::std::find(it->second.begin(), it->second.end(), mesh);
    }
    it->second.erase(iter);
  }

private:
  ::std::map<Material, RenderableCont> m_RenderGroups;
  ::std::map<Material, RenderableCont> m_RenderGroupsAlpha;
};
} // namespace upgl