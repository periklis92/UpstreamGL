#pragma once
#include <cstdint>
#include <functional>

#include <glm/glm.hpp>

#include "GLHelper.h"

#define UPGL_SHADER_MVP_NAME "_ModelViewProjection"
#define UPGL_SHADER_COLOR_NAME "_MaterialColor"
#define UPGL_SHADER_BONE_TRANSFORM_NAME "_BoneTransforms"
#define UPGL_SHADER_MAIN_TEX_NAME "_MainTexture"

namespace upgl {
class ShaderBuilder;
class ShaderResource;

class Shader {
public:
  Shader();
  Shader(const Shader &shader);
  Shader &operator=(const Shader &shader);

  Shader(Shader &&shader) noexcept;
  Shader &operator=(Shader &&shader) noexcept;
  ~Shader() = default;

  bool operator==(const Shader &other) const { return m_ProgramId == other.m_ProgramId; }
  bool operator<(const Shader &other) const { return m_ProgramId < other.m_ProgramId; }

  void Bind();
  void Delete();

  void SetMat4(const char *name, const glm::mat4 &matrix);
  void SetMat4(const char *name, const glm::mat4 *matrix, size_t num);
  void SetInt(const char *name, int value);
  void SetInt(const char *name, int *value, std::size_t count);
  void SetFloat(const char *name, float *value, std::size_t count);

private:
  uint32_t m_ProgramId;

  friend class Material;
  friend class ShaderBuilder;
  friend class ShaderResource;
  friend struct std::hash<Shader>;
};
} // namespace upgl

namespace std {
template <> struct hash<upgl::Shader> {
  size_t operator()(const upgl::Shader &shader) const noexcept {
    return hash<uint32_t>{}(shader.m_ProgramId);
  }
};
} // namespace std