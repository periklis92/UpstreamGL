#pragma once

#include <vector>

#include "Bone.h"
#include "Shader.h"
#include "Vertex.h"
#include <UpstreamGL/Math/AABB.h>

class AnimationClip;

class MeshData
{
public:
	MeshData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const AABB& aabb);
	MeshData(const std::vector<SkinnedVertex>& vertices, const std::vector<uint32_t>& indices, const AABB& aabb, Armature* armature);

	MeshData(const MeshData&) = delete;
	MeshData& operator=(const MeshData&) = delete;

	MeshData(MeshData&& other);
	MeshData& operator=(MeshData&&);

	~MeshData();

	void Draw();

	Armature* GetArmature() const { return m_Armature; }
	size_t GetNumIndices() { return m_NumIndices; }

	const AABB GetBounds() const { return m_AABB; }
	bool IsSkinned() const { return m_Armature != nullptr; }

	void Bind();
	void Unbind();

private:
	uint32_t m_NumIndices;
	AABB m_AABB;
	uint32_t m_VertexArray, m_VertexBuffer, m_ElementBuffer;
	Armature* m_Armature;
};