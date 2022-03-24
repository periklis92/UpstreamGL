#pragma once

#include <assimp/aabb.h>
#include <glm/glm.hpp>

#include "AssimpGLMHelper.h"

namespace upgl {
struct AABB {
  AABB() = default;

  AABB(glm::vec3 min, glm::vec3 max) : Min(min), Max(max) {}

  AABB(const aiAABB &aabb)
      : Min(AssimpGLMHelper::ConvertToGLM(aabb.mMin)),
        Max(AssimpGLMHelper::ConvertToGLM(aabb.mMax)) {}

  inline const glm::vec3 GetSize() const { return Max - Min; }

  inline bool IsValid() const { return Min.x < Max.x && Min.y < Max.y && Min.z < Max.z; }

  glm::vec3 Min{999999};
  glm::vec3 Max{-999999};
};
} // namespace upgl