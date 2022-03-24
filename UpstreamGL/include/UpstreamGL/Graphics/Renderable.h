#pragma once

#include <glm/mat4x4.hpp>

#include <UpstreamGL/Graphics/Material.h>
#include <UpstreamGL/Math/AABB.h>

namespace upgl {
class Camera;
class Node;
class Shader;
class MeshData;
class Texture2D;

class Renderable {
public:
  virtual ~Renderable() = default;

  virtual void Render(glm::mat4 viewProj) = 0;
  virtual Node *GetNode() = 0;

  virtual const MaterialInstance &GetMateriaInstance() const { return *m_Material; }
  virtual void SetMateriaInstance(MaterialInstance *material) { m_Material = material; }
  virtual AABB GetBounds() const = 0;

protected:
  MaterialInstance *m_Material{nullptr};
};
} // namespace upgl