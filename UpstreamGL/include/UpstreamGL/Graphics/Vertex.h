#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <UpstreamGL/Config.h>

namespace upgl {
struct Vertex {
  glm::vec3 position{0, 0, 0};
  glm::vec3 normal{0, 0, 0};
  glm::vec2 uv{0, 0};
  glm::u8vec4 color{255, 255, 255, 255};
  glm::vec3 tangent{0, 0, 0};
  glm::vec3 bitangent{0, 0, 0};
};

struct UIVertex {
  glm::vec3 position{0, 0, 0};
  glm::vec2 uv{0, 0};
  glm::u8vec4 color{0, 0, 0, 255};
};

struct SkinnedVertex {
  glm::vec3 position{0, 0, 0};
  glm::vec3 normal{0, 0, 0};
  glm::vec2 uv{0, 0};
  glm::u8vec4 color{255, 255, 255, 255};
  glm::vec3 tangent{0, 0, 0};
  glm::vec3 bitangent{0, 0, 0};
  float boneIds[UPGL_MAX_BONES_PER_VERTEX];
  float boneWeights[UPGL_MAX_BONES_PER_VERTEX];

  inline SkinnedVertex() {
    for (int i = 0; i < UPGL_MAX_BONES_PER_VERTEX; ++i) {
      boneIds[i] = -1;
      boneWeights[i] = -1;
    }
  }
};
} // namespace upgl