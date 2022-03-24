#include <cstdint>

#include <glad/glad.h>

#include <UpstreamGL/Graphics/Animation.h>
#include <UpstreamGL/Graphics/MeshData.h>

namespace upgl {
MeshData::MeshData(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices,
                   const AABB &aabb)
    : m_Vertices(vertices), m_Indices(indices), m_AABB(aabb) {}

MeshData::MeshData(const MeshData &other) { *this = other; }

MeshData &MeshData::operator=(const MeshData &other) {
  m_Vertices = other.m_Vertices;
  m_Indices = other.m_Indices;
  m_AABB = other.m_AABB;
  return *this;
}

MeshData::MeshData(MeshData &&other) noexcept { *this = std::move(other); }

MeshData &MeshData::operator=(MeshData &&other) noexcept {
  m_Vertices = std::move(other.m_Vertices);
  m_Indices = std::move(other.m_Indices);
  m_AABB = std::move(other.m_AABB);
  return *this;
}

SkinnedMeshData::SkinnedMeshData(const std::vector<SkinnedVertex> &vertices,
                                 const std::vector<uint32_t> &indices, const AABB &aabb)
    : m_Vertices(vertices), m_Indices(indices), m_AABB(aabb) {}

SkinnedMeshData::SkinnedMeshData(SkinnedMeshData &&other) noexcept { *this = std::move(other); }

SkinnedMeshData &SkinnedMeshData::operator=(SkinnedMeshData &&other) noexcept {
  m_Vertices = std::move(other.m_Vertices);
  m_Indices = std::move(other.m_Indices);
  m_AABB = std::move(other.m_AABB);
  return *this;
}

MeshData MeshData::CreateQuad(glm::vec2 size, glm::vec2 center, glm::vec4 uv) {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  vertices.insert(
      vertices.begin(),
      {{
           {center.x, center.y, 0.f}, {0, 0, -1}, {uv.x, uv.y} // Top Left
       },
       {
           {center.x + size.x, center.y, 0.f}, {0, 0, -1}, {uv.z, uv.y} // Top Right
       },
       {
           {center.x, center.y + size.y, 0.f}, {0, 0, -1}, {uv.x, uv.w} // Bottom Left
       },
       {
           {center.x + size.x, center.y + size.y, 0.f}, {0, 0, -1}, {uv.z, uv.w} // Bottom Right
       }});

  indices.insert(indices.begin(), {0, 1, 2, 1, 3, 2});

  return MeshData(vertices, indices,
                  AABB{glm::vec3{center - size / 2.f, 0.f}, glm::vec3{center + size / 2.f, 0.f}});
}

MeshData MeshData::Create9SliceQuad(glm::vec2 size, float cornerPct, glm::vec2 center) {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  vertices.insert(
      vertices.begin(),
      {{// Top
        {center.x - size.x / 2.f, center.y - size.y / 2.f, 0.f},
        {0, 0, -1},
        {0.f, 0.f}},
       {{center.x - (size.x / 2.f) + (size.x / 2.f) * cornerPct, center.y - size.y / 2.f, 0.f},
        {0, 0, -1},
        {cornerPct, 0.f}},
       {{center.x + (size.x / 2.f) - (size.x / 2.f) * cornerPct, center.y - size.y / 2.f, 0.f},
        {0, 0, -1},
        {1.f - cornerPct, 0.f}},
       {{center.x + size.x / 2.f, center.y - size.y / 2.f, 0.f}, {0, 0, -1}, {1.f, 0.f}},
       {// Upper
        {center.x - size.x / 2.f, center.y - size.y / 2.f + (size.y / 2.f) * cornerPct, 0.f},
        {0, 0, -1},
        {0, cornerPct}},
       {{center.x - (size.x / 2.f) + (size.x / 2.f) * cornerPct,
         center.y - size.y / 2.f + (size.y / 2.f) * cornerPct, 0.f},
        {0, 0, -1},
        {cornerPct, cornerPct}},
       {{center.x + (size.x / 2.f) - (size.x / 2.f) * cornerPct,
         center.y - size.y / 2.f + (size.y / 2.f) * cornerPct, 0.f},
        {0, 0, -1},
        {1.f - cornerPct, cornerPct}},
       {{center.x + size.x / 2.f, center.y - size.y / 2.f + (size.y / 2.f) * cornerPct, 0.f},
        {0, 0, -1},
        {1.f, cornerPct}},
       {// Lower
        {center.x - size.x / 2.f, center.y + size.y / 2.f - (size.y / 2.f) * cornerPct, 0.f},
        {0, 0, -1},
        {0, 1.f - cornerPct}},
       {{center.x - (size.x / 2.f) + (size.x / 2.f) * cornerPct,
         center.y + size.y / 2.f - (size.y / 2.f) * cornerPct, 0.f},
        {0, 0, -1},
        {cornerPct, 1.f - cornerPct}},
       {{center.x + (size.x / 2.f) - (size.x / 2.f) * cornerPct,
         center.y + size.y / 2.f - (size.y / 2.f) * cornerPct, 0.f},
        {0, 0, -1},
        {1.f - cornerPct, 1.f - cornerPct}},
       {{center.x + size.x / 2.f, center.y + size.y / 2.f - (size.y / 2.f) * cornerPct, 0.f},
        {0, 0, -1},
        {1.f, 1.f - cornerPct}},
       {// Bottom
        {center.x - size.x / 2.f, center.y + size.y / 2.f, 0.f},
        {0, 0, -1},
        {0.f, 1.f}},
       {{center.x - (size.x / 2.f) + (size.x / 2.f) * cornerPct, center.y + size.y / 2.f, 0.f},
        {0, 0, -1},
        {cornerPct, 1.f}},
       {{center.x + (size.x / 2.f) - (size.x / 2.f) * cornerPct, center.y + size.y / 2.f, 0.f},
        {0, 0, -1},
        {1.f - cornerPct, 1.f}},
       {{center.x + size.x / 2.f, center.y + size.y / 2.f, 0.f}, {0, 0, -1}, {1.f, 1.f}}});

  indices.insert(indices.begin(), {0, 1, 2, 1, 3, 2});

  return MeshData(vertices, indices,
                  AABB{glm::vec3{center - size / 2.f, 0.f}, glm::vec3{center + size / 2.f, 0.f}});
}

} // namespace upgl