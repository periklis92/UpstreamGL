#pragma once

#include <utility>

#include "Shader.h"

namespace upgl {
class ShaderBuilder {
public:
  ShaderBuilder();
  ShaderBuilder &AddShader(const char *shader, ShaderType shaderType);
  Shader Build();

private:
  static void CheckShaderCompilationError(uint32_t shaderId);
  static void CheckProgramLinkError(uint32_t programId);

private:
  Shader m_Shader;
};
} // namespace upgl