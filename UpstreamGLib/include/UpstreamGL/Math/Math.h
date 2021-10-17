#pragma once

#include <glm/glm.hpp>
#include "glm/gtx/quaternion.hpp"

namespace Math
{
	glm::vec3 GetTranslation(const glm::mat4& matrix);
	glm::vec3 GetScale(const glm::mat4& matrix);
	glm::mat3 GetRotationMatrix(const glm::mat4& matrix);	
	glm::quat GetRotationQuat(const glm::mat4& matrix);
};