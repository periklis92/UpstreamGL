#pragma once

#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <UpstreamGL/Config.h>

struct Vertex
{
	glm::vec3 position{0, 0, 0};
	glm::vec3 normal{ 0, 0, 0 };
	glm::vec2 uv{ 0, 0 };
	glm::u8vec4 color{ 255, 255, 255, 255 };
	glm::vec3 tangent{ 0, 0, 0 };
	glm::vec3 bitangent{ 0, 0, 0 };
};

struct SkinnedVertex
{
	glm::vec3 position{ 0, 0, 0 };
	glm::vec3 normal{ 0, 0, 0 };
	glm::vec2 uv{ 0, 0 };
	glm::u8vec4 color{ 255, 255, 255, 255 };
	glm::vec3 tangent{ 0, 0, 0 };
	glm::vec3 bitangent{ 0, 0, 0 };
	float boneIds[UPGL_MAX_BONES_PER_VERTEX];
	float boneWeights[UPGL_MAX_BONES_PER_VERTEX];

	inline SkinnedVertex()
	{
		for (int i = 0; i < UPGL_MAX_BONES_PER_VERTEX; ++i)
		{
			boneIds[i] = -1;
			boneWeights[i] = -1;
		}
	}
};