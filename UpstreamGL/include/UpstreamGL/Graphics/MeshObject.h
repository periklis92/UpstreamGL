#pragma once

#include <cstdint>
#include <vector>

#include "MeshData.h"
#include "Vertex.h"

namespace upgl {
class MeshObject;
class IndexedMeshObject;

class MeshObjectFactory {
public:
  static IndexedMeshObject CreateUIMesh(const MeshData &mesh);
  static IndexedMeshObject CreateIndexedMesh(const MeshData &mesh);
  static IndexedMeshObject CreateSkinnedIndexedMesh(const SkinnedMeshData &mesh);
};

class MeshObject {
public:
  MeshObject() = default;

  virtual void Bind() = 0;
  virtual void Render() = 0;
  virtual void Delete() = 0;
};

class IndexedMeshObject : public MeshObject {
public:
  IndexedMeshObject() = default;

  virtual void Bind() override;
  virtual void Render() override;
  virtual void Delete() override;

private:
  uint32_t m_NumIndices{0}, m_VertexArray{0}, m_VertexBuffer{0}, m_ElementBuffer{0};

  friend class MeshObjectFactory;
};
} // namespace upgl