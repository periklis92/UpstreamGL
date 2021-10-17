#include <UpstreamGL/Graphics/Bone.h>

#include <memory>
#include <assimp/scene.h>
#include <UpstreamGL/Math/AssimpGLMHelper.h>
#include <UpstreamGL/Graphics/Animation.h>

Bone::Bone()
	:m_Name(), m_Id(0), m_Transform(1.f), m_Offset(1.f), m_FinalTransform(1.f), m_Parent(nullptr)
{
}

void Bone::AddChild(Bone* child)
{
	m_Children.push_back(child);
	child->m_Parent = this;
}

void Bone::UpdateFinalTransform(const std::map<std::string, glm::mat4>& transforms, const glm::mat4& parentTransform)
{
	auto it = transforms.find(m_Name);
	glm::mat4 nodeTransform = m_Transform;
	
	if (it != transforms.end())
	{
		nodeTransform = it->second;
	}

	glm::mat4 globalTransform = parentTransform * nodeTransform;
	m_FinalTransform = globalTransform * m_Offset;

	for (auto& child : m_Children)
		child->UpdateFinalTransform(transforms, globalTransform);

}

void Bone::Reset(const glm::mat4& parentTransform)
{
	glm::mat4 globalTransform = parentTransform * m_Transform;
	m_FinalTransform = globalTransform * m_Offset;

	for (auto& child : m_Children)
		child->Reset(globalTransform);
}

Armature::Armature(const aiNode* root, const aiMesh* mesh)
	:m_RootBone(), m_NumBones(mesh->mNumBones)
{
	Bone* bonesPool = new Bone[m_NumBones];
	m_VertexWeights.resize(mesh->mNumVertices);
	for (uint32_t i = 0; i < m_NumBones; ++i)
	{
		bonesPool[i].m_Id = i;
		bonesPool[i].m_Name = mesh->mBones[i]->mName.C_Str();
		bonesPool[i].m_Offset = AssimpGLMHelper::ConvertToGLM(mesh->mBones[i]->mOffsetMatrix);
		for (uint32_t j = 0; j < mesh->mBones[i]->mNumWeights; ++j)
		{
			aiVertexWeight& w = mesh->mBones[i]->mWeights[j];

			VertexWeight& vw = m_VertexWeights[w.mVertexId];
			if (vw.NumBones < UPGL_MAX_BONES_PER_VERTEX)
			{
				vw.BoneIds[vw.NumBones] = i;
				vw.Weight[vw.NumBones] = w.mWeight;
				++vw.NumBones;
			}
		}
		m_Bones[bonesPool[i].m_Name] = &bonesPool[i];
	}

	for (uint32_t c = 0; c < m_NumBones; ++c)
	{
		const aiNode* node = root->FindNode(mesh->mBones[c]->mName.C_Str());
		for (uint32_t j = 0; j < node->mNumChildren; ++j)
		{
			auto it = m_Bones.find(node->mChildren[j]->mName.C_Str());
			if (it != m_Bones.end())
				m_Bones[mesh->mBones[c]->mName.C_Str()]->AddChild(it->second);
		}
	}

	for (auto& bone : m_Bones)
	{
		if (!bone.second->GetParent())
		{
			m_RootBone = bone.second;
			break;
		}
	}
}
void Armature::UpdateFromAnimation(const AnimationClip& clip, float time)
{
	m_RootBone->UpdateFinalTransform(clip.GetChannelsTransforms(time), glm::mat4(1.f));
}

void Armature::Reset()
{
	m_RootBone->Reset(glm::mat4(1.f));
}

Bone* Armature::FindBone(const std::string& name)
{
	auto it = m_Bones.find(name);
	if (it != m_Bones.end()) return it->second;
	return nullptr;
}

std::vector<glm::mat4> Armature::GetBoneTransforms()
{
	std::vector<glm::mat4> transforms;
	transforms.resize(m_NumBones);
	for (auto& bone : m_Bones)
	{
		transforms[bone.second->m_Id] = bone.second->m_FinalTransform;
	}
	return transforms;
}