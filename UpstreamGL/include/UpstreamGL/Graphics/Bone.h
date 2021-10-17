#pragma once

#include <map>
#include <string>
#include <cstdint>
#include <vector>

#include <UpstreamGL/Config.h>
#include <glm/glm.hpp>

struct aiNode;
struct aiMesh;
class Armature;
class AnimationClip;
struct Vertex;

struct BoneWeight
{
	uint32_t VertexId;
	float Weight = -1.f;
};

struct VertexWeight
{
	int BoneIds[UPGL_MAX_BONES_PER_VERTEX];
	float Weight[UPGL_MAX_BONES_PER_VERTEX];
	uint32_t NumBones = 0;
};

class Bone
{
public:
	Bone();
	~Bone() = default;

	const std::string& GetName() const { return m_Name; }
	const glm::mat4& GetLocalTransform() const { return m_Transform; }

	uint32_t GetId() const { return m_Id; }
	void AddChild(Bone* child);
	Bone* GetParent() const { return m_Parent; }
	//void FindInChildren(uint32_t vertexId, std::vector<VertexWeight>& bones) const;
private:
	void UpdateFinalTransform(const std::map<std::string, glm::mat4>& transforms, const glm::mat4& parentTransform);
	void Reset(const glm::mat4& parentTransform);

	friend class Armature;

private:
	std::string m_Name;
	uint32_t m_Id;
	glm::mat4 m_Transform;
	glm::mat4 m_Offset;
	glm::mat4 m_FinalTransform;
	Bone* m_Parent;
	std::vector<Bone*> m_Children;
	std::vector<BoneWeight> m_Weights;
};

class Armature
{
public:
	Armature(const aiNode* root, const aiMesh* mesh);

	uint32_t GetNumBones() const { return m_NumBones; }
	void UpdateFromAnimation(const AnimationClip& clip, float time);
	void Reset();
	Bone* FindBone(const std::string& name);
	std::vector<glm::mat4> GetBoneTransforms();
	const std::vector<VertexWeight>& GetVertexWeights() const { return m_VertexWeights; }
	std::vector<VertexWeight>& GetVertexWeights() { return m_VertexWeights; }

private:
	Bone* m_RootBone;
	uint32_t m_NumBones;
	std::map<std::string, Bone*> m_Bones;
	std::vector<VertexWeight> m_VertexWeights;
};
