#pragma once

#include "assimp/quaternion.h"
#include "assimp/vector3.h"
#include "assimp/matrix4x4.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

class AssimpGLMHelper
{
public:
	static inline glm::mat4 ConvertToGLM(const aiMatrix4x4& from)
	{
		glm::mat4 res;
		res[0][0] = from.a1; res[1][0] = from.a2; res[2][0] = from.a3; res[3][0] = from.a4;
		res[0][1] = from.b1; res[1][1] = from.b2; res[2][1] = from.b3; res[3][1] = from.b4;
		res[0][2] = from.c1; res[1][2] = from.c2; res[2][2] = from.c3; res[3][2] = from.c4;
		res[0][3] = from.d1; res[1][3] = from.d2; res[2][3] = from.d3; res[3][3] = from.d4;
		return res;
	}

	static inline glm::vec3 ConvertToGLM(const aiVector3D& vec)
	{
		return glm::vec3(vec.x, vec.y, vec.z);
	}

	static inline glm::quat ConvertToGLM(const aiQuaternion& quat)
	{
		return glm::quat(quat.w, quat.x, quat.y, quat.z);
	}
};