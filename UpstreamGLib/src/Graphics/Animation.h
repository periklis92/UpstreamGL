#pragma once

#include <vector>
#include <cstdint>
#include <string>

#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include <map>

struct aiNodeAnim;
struct aiAnimation;

struct KeyPosition
{
	glm::vec3 Position;
	float TimeStamp;
};

struct KeyRotation
{
	glm::quat Rotation;
	float TimeStamp;
};

struct KeyScale
{
	glm::vec3 Scale;
	float TimeStamp;
};

struct AnimationChannel
{
public:
	AnimationChannel(const std::string& name, int32_t Id, const aiNodeAnim* channel);

	const std::string& GetName() const { return m_Name; }
	int32_t GetId() { return m_BoneId; }

	glm::mat4 GetTransform(float time) const;

private:
	int32_t GetPosIndex(float time) const;
	int32_t GetRotIndex(float time) const;
	int32_t GetScaleIndex(float time) const;

	glm::mat4 InterpolatePosition(float time) const;
	glm::mat4 InterpolateRotation(float time) const;
	glm::mat4 InterpolateScale(float time) const;

	float GetFactor(float from, float to, float current) const;
private:
	int32_t m_BoneId;
	std::string m_Name;
	std::vector<KeyPosition> m_PositionKeys;
	std::vector<KeyRotation> m_RotationKeys;
	std::vector<KeyScale> m_ScaleKeys;
};

class AnimationClip
{
public:
	AnimationClip(const std::string& name, const aiAnimation* animation);
	void AddChannel(const std::string& name, int32_t id, const aiNodeAnim* channel);

	const std::vector<AnimationChannel>& GetChannels() const { return m_Channels; }
	float GetDuration() const { return m_Duration; }
	float GetTicksPerSecond() const { return m_TicksPerSecond; }
	const std::map<std::string, glm::mat4> GetChannelsTransforms(float time) const;

private:
	std::string m_Name;
	std::vector<AnimationChannel> m_Channels;
	float m_Duration;
	float m_TicksPerSecond;
};

class Animation
{
public:
	Animation() = default;
	~Animation() = default;

	std::map<std::string, AnimationClip>& GetClips() { return m_Clips; }
	void AddClip(const std::string& name, AnimationClip&& clip);
	void Clear() { m_Clips.clear(); }
private:
	std::map<std::string, AnimationClip> m_Clips{};
};