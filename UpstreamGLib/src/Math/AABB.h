#pragma once

#include "glm/glm.hpp"
#include "assimp/aabb.h"
#include "AssimpGLMHelper.h"

struct AABB
{
	AABB() = default;

	AABB(const aiAABB& aabb)
		:Min(AssimpGLMHelper::ConvertToGLM(aabb.mMin)), Max(AssimpGLMHelper::ConvertToGLM(aabb.mMax)) {}

	inline const glm::vec3 GetSize() const
	{
		return Max - Min;
	}

	inline bool IsValid() const
	{
		return Min.x < Max.x && Min.y < Max.y && Min.z < Max.z;
	}

	glm::vec3 Min{999999};
	glm::vec3 Max{-999999};
};