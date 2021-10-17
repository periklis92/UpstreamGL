#include <UpstreamGL/Graphics/MeshData.h>

#include <cstdint>

#include <UpstreamGL/Graphics/Animation.h>
#include "glad/glad.h"

MeshData::MeshData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const AABB& aabb)
	:m_NumIndices(indices.size()), m_AABB(aabb), m_VertexArray(0), m_VertexBuffer(0), m_ElementBuffer(0), m_Armature(nullptr)
{
	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);
	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glGenBuffers(1, &m_ElementBuffer);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
		((void*)vertices.data()), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	glEnableVertexAttribArray(5);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		((void*)indices.data()), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

MeshData::MeshData(const std::vector<SkinnedVertex>& vertices, const std::vector<uint32_t>& indices, const AABB& aabb, Armature* armature)
	:m_NumIndices(indices.size()), m_AABB(aabb), m_VertexArray(0), m_VertexBuffer(0), m_ElementBuffer(0), m_Armature(armature)
{
	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);
	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glGenBuffers(1, &m_ElementBuffer);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SkinnedVertex),
		((void*)vertices.data()), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, uv));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, color));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(SkinnedVertex, tangent));
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(SkinnedVertex, bitangent));
	glEnableVertexAttribArray(5);

	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, boneIds));
	glEnableVertexAttribArray(6);

	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, boneWeights));
	glEnableVertexAttribArray(7);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t),
		((void*)indices.data()), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

MeshData::MeshData(MeshData&& other)
{
	*this = std::move(other);
}

MeshData& MeshData::operator=(MeshData&& other)
{
	m_AABB 					= other.m_AABB;
	m_Armature 				= other.m_Armature;
	m_ElementBuffer 		= other.m_ElementBuffer;
	m_VertexArray 			= other.m_VertexArray;
	m_VertexBuffer 			= other.m_VertexBuffer;
	m_NumIndices 			= other.m_NumIndices;
	other.m_ElementBuffer	= 0;
	other.m_VertexArray 	= 0;
	other.m_VertexBuffer 	= 0;
	return *this;
}

MeshData::~MeshData()
{
	if (m_ElementBuffer)
		glDeleteBuffers(1, &m_ElementBuffer);
	if (m_VertexBuffer)
		glDeleteBuffers(1, &m_VertexBuffer);
	if (m_VertexArray)
		glDeleteVertexArrays(1, &m_VertexArray);
}

void MeshData::Draw()
{
	Bind();
	glDrawElements(GL_TRIANGLES, m_NumIndices, GL_UNSIGNED_INT, 0);
	Unbind();
}

void MeshData::Bind()
{
	glBindVertexArray(m_VertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);
}

void MeshData::Unbind()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}