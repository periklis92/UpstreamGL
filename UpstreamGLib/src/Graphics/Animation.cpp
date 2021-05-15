#include "Animation.h"
#include "Math/AssimpGLMHelper.h"
#include "assimp/anim.h"

AnimationChannel::AnimationChannel(const std::string& name, int32_t Id, const aiNodeAnim* channel)
	:m_Name(name), m_BoneId(Id)
{
	for(uint32_t pos = 0; pos < channel->mNumPositionKeys; ++pos)
	{
		KeyPosition Pkey;
		Pkey.Position = AssimpGLMHelper::ConvertToGLM(channel->mPositionKeys[pos].mValue);
		Pkey.TimeStamp = channel->mPositionKeys[pos].mTime;
		m_PositionKeys.push_back(std::move(Pkey));
	}

	for (uint32_t rot = 0; rot < channel->mNumRotationKeys; ++rot)
	{
		KeyRotation Pkey;
		Pkey.Rotation = AssimpGLMHelper::ConvertToGLM(channel->mRotationKeys[rot].mValue);
		Pkey.TimeStamp = channel->mPositionKeys[rot].mTime;
		m_RotationKeys.push_back(std::move(Pkey));
	}

	for (uint32_t scl = 0; scl < channel->mNumScalingKeys; ++scl)
	{
		KeyScale Pkey;
		Pkey.Scale = AssimpGLMHelper::ConvertToGLM(channel->mScalingKeys[scl].mValue);
		Pkey.TimeStamp = channel->mPositionKeys[scl].mTime;
		m_ScaleKeys.push_back(std::move(Pkey));
	}
}

glm::mat4 AnimationChannel::GetTransform(float time) const
{
	glm::mat4 position = InterpolatePosition(time);
	glm::mat4 rotation = InterpolateRotation(time);
	glm::mat4 scale = InterpolateScale(time);
	return position * rotation * scale;
}

int32_t AnimationChannel::GetPosIndex(float time) const
{
	for (int i = 0; i < m_PositionKeys.size() - 1; ++i)
	{
		if (time < m_PositionKeys[i + 1].TimeStamp)
			return i;
	}
	return -1;
}

int32_t AnimationChannel::GetRotIndex(float time) const
{
	for (int i = 0; i < m_RotationKeys.size() - 1; ++i)
	{
		if (time < m_RotationKeys[i + 1].TimeStamp)
			return i;
	}
	return -1;
}

int32_t AnimationChannel::GetScaleIndex(float time) const
{
	for (int i = 0; i < m_ScaleKeys.size() - 1; ++i)
	{
		if (time < m_ScaleKeys[i + 1].TimeStamp)
			return i;
	}
	return -1;
}

glm::mat4 AnimationChannel::InterpolatePosition(float time) const
{
	if (m_PositionKeys.size() == 1) return glm::translate(glm::mat4(1.f), m_PositionKeys[0].Position);
	if (m_PositionKeys.size() > 1)
	{
		int32_t index0 = GetPosIndex(time);
		int32_t index1 = index0 + 1;

		float t = GetFactor(m_PositionKeys[index0].TimeStamp, 
					m_PositionKeys[index1].TimeStamp, time);

		glm::vec3 finalPos = glm::mix(m_PositionKeys[index0].Position,
						m_PositionKeys[index1].Position, t);

		return glm::translate(glm::mat4(1.f), finalPos);
	}

	return glm::mat4(1.f);
}

glm::mat4 AnimationChannel::InterpolateRotation(float time) const
{
	if (m_RotationKeys.size() == 1) return glm::mat4_cast(glm::normalize(m_RotationKeys[0].Rotation));
	if (m_PositionKeys.size() > 1)
	{
		int32_t index0 = GetRotIndex(time);
		int32_t index1 = index0 + 1;

		float t = GetFactor(m_RotationKeys[index0].TimeStamp,
			m_RotationKeys[index1].TimeStamp, time);

		glm::quat finalRot = glm::slerp(m_RotationKeys[index0].Rotation,
			m_RotationKeys[index1].Rotation, t);

		return  glm::toMat4(glm::normalize(finalRot));
	}

	return glm::mat4(1.f);
}

glm::mat4 AnimationChannel::InterpolateScale(float time) const
{
	if (m_ScaleKeys.size() == 1) return glm::scale(glm::mat4(1.f), m_ScaleKeys[0].Scale);
	if (m_ScaleKeys.size() > 1)
	{
		int32_t index0 = GetScaleIndex(time);
		int32_t index1 = index0 + 1;

		float t = GetFactor(m_ScaleKeys[index0].TimeStamp,
			m_ScaleKeys[index1].TimeStamp, time);

		glm::vec3 finalScale = glm::mix(m_ScaleKeys[index0].Scale,
			m_ScaleKeys[index1].Scale, t);

		return glm::scale(glm::mat4(1.f), finalScale);
	}

	return glm::mat4(1.f);
}

float AnimationChannel::GetFactor(float from, float to, float current) const
{
	float diff1 = current - from;
	float diff2 = to - from;
	return diff1 / diff2;
}

AnimationClip::AnimationClip(const std::string& name, const aiAnimation* animation)
	:m_Name(name), m_Duration(animation->mDuration), m_TicksPerSecond(animation->mTicksPerSecond)
{
	for (uint32_t i = 0; i < animation->mNumChannels; ++i)
	{
		AddChannel(animation->mChannels[i]->mNodeName.C_Str(), 0, animation->mChannels[i]);
	}
}

void AnimationClip::AddChannel(const std::string& name, int32_t id, const aiNodeAnim* channel)
{
	m_Channels.push_back(AnimationChannel(name, id, channel));
}

const std::map<std::string, glm::mat4> AnimationClip::GetChannelsTransforms(float time) const
{
	std::map<std::string, glm::mat4> ret;
	for (auto& channel : m_Channels)
	{
		ret[channel.GetName()] = channel.GetTransform(time);
	}
	return ret;
}

void Animation::AddClip(const std::string& name, AnimationClip&& clip)
{
	m_Clips.emplace(name, std::move(clip));
}