#pragma once

#include <vector>

#include <UpstreamGL/Math/AABB.h>

#include "Bone.h"
#include "Shader.h"
#include "Vertex.h"

namespace upgl {
class AnimationClip;

class MeshData {
public:
  MeshData() = default;
  MeshData(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices,
           const AABB &aabb);

  MeshData(const MeshData &);
  MeshData &operator=(const MeshData &);

  MeshData(MeshData &&other) noexcept;
  MeshData &operator=(MeshData &&) noexcept;

  const std::vector<Vertex> &GetVertices() const { return m_Vertices; }
  const std::vector<uint32_t> &GetIndices() const { return m_Indices; }

  void Clear() {
    m_Vertices.clear();
    m_Indices.clear();
  }

  AABB GetBounds() const { return m_AABB; }

public:
  static MeshData CreateQuad(glm::vec2 size, glm::vec2 center = {0.f, 0.f},
                             glm::vec4 uv = {0.f, 0.f, 1.f, 1.f});
  static MeshData Create9SliceQuad(glm::vec2 size, float cornerPct, glm::vec2 center = {0.f, 0.f});

private:
  std::vector<Vertex> m_Vertices;
  std::vector<uint32_t> m_Indices;
  AABB m_AABB;
};

class SkinnedMeshData {
public:
  SkinnedMeshData() = default;
  SkinnedMeshData(const std::vector<SkinnedVertex> &vertices, const std::vector<uint32_t> &indices,
                  const AABB &aabb);

  SkinnedMeshData(const SkinnedMeshData &) = delete;
  SkinnedMeshData &operator=(const SkinnedMeshData &) = delete;

  SkinnedMeshData(SkinnedMeshData &&other) noexcept;
  SkinnedMeshData &operator=(SkinnedMeshData &&) noexcept;

  const std::vector<SkinnedVertex> &GetVertices() const { return m_Vertices; }
  const std::vector<uint32_t> &GetIndices() const { return m_Indices; }

  void Clear() {
    m_Vertices.clear();
    m_Indices.clear();
  }

  AABB GetBounds() const { return m_AABB; }

private:
  std::vector<SkinnedVertex> m_Vertices;
  std::vector<uint32_t> m_Indices;
  AABB m_AABB;
};
} // namespace upgl