#include <glad/glad.h>

#include <UpstreamGL/Graphics/MeshObject.h>

namespace upgl {
IndexedMeshObject MeshObjectFactory::CreateUIMesh(const MeshData &mesh) {
  IndexedMeshObject obj;
  obj.m_NumIndices = mesh.GetIndices().size();
  glGenVertexArrays(1, &obj.m_VertexArray);
  glBindVertexArray(obj.m_VertexArray);
  glGenBuffers(1, &obj.m_VertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, obj.m_VertexBuffer);
  glGenBuffers(1, &obj.m_ElementBuffer);

  glBufferData(GL_ARRAY_BUFFER, mesh.GetVertices().size() * sizeof(Vertex),
               ((void *)mesh.GetVertices().data()), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
                        (void *)offsetof(Vertex, color));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.m_ElementBuffer);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndices().size() * sizeof(unsigned int),
               ((void *)mesh.GetIndices().data()), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  return obj;
}

IndexedMeshObject MeshObjectFactory::CreateIndexedMesh(const MeshData &mesh) {
  IndexedMeshObject obj;
  obj.m_NumIndices = mesh.GetIndices().size();
  glGenVertexArrays(1, &obj.m_VertexArray);
  glBindVertexArray(obj.m_VertexArray);
  glGenBuffers(1, &obj.m_VertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, obj.m_VertexBuffer);
  glGenBuffers(1, &obj.m_ElementBuffer);

  glBufferData(GL_ARRAY_BUFFER, mesh.GetVertices().size() * sizeof(Vertex),
               ((void *)mesh.GetVertices().data()), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
                        (void *)offsetof(Vertex, color));
  glEnableVertexAttribArray(3);

  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, tangent));
  glEnableVertexAttribArray(4);

  glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, bitangent));
  glEnableVertexAttribArray(5);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.m_ElementBuffer);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndices().size() * sizeof(unsigned int),
               ((void *)mesh.GetIndices().data()), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  return obj;
}

IndexedMeshObject MeshObjectFactory::CreateSkinnedIndexedMesh(const SkinnedMeshData &mesh) {
  IndexedMeshObject obj;
  obj.m_NumIndices = mesh.GetIndices().size();
  glGenVertexArrays(1, &obj.m_VertexArray);
  glBindVertexArray(obj.m_VertexArray);
  glGenBuffers(1, &obj.m_VertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, obj.m_VertexBuffer);
  glGenBuffers(1, &obj.m_ElementBuffer);

  glBufferData(GL_ARRAY_BUFFER, mesh.GetVertices().size() * sizeof(SkinnedVertex),
               ((void *)mesh.GetVertices().data()), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex),
                        (void *)offsetof(SkinnedVertex, position));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex),
                        (void *)offsetof(SkinnedVertex, normal));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex),
                        (void *)offsetof(SkinnedVertex, uv));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(SkinnedVertex),
                        (void *)offsetof(SkinnedVertex, color));
  glEnableVertexAttribArray(3);

  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(SkinnedVertex, tangent));
  glEnableVertexAttribArray(4);

  glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(SkinnedVertex, bitangent));
  glEnableVertexAttribArray(5);

  glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex),
                        (void *)offsetof(SkinnedVertex, boneIds));
  glEnableVertexAttribArray(6);

  glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex),
                        (void *)offsetof(SkinnedVertex, boneWeights));
  glEnableVertexAttribArray(7);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.m_ElementBuffer);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndices().size() * sizeof(uint32_t),
               ((void *)mesh.GetIndices().data()), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  return obj;
}

void IndexedMeshObject::Bind() {
  glBindVertexArray(m_VertexArray);
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);
}

void IndexedMeshObject::Render() { glDrawElements(GL_TRIANGLES, m_NumIndices, GL_UNSIGNED_INT, 0); }

void IndexedMeshObject::Delete() {
  glDeleteBuffers(1, &m_VertexBuffer);
  glDeleteBuffers(1, &m_ElementBuffer);
  glDeleteVertexArrays(1, &m_VertexArray);
}
} // namespace upgl