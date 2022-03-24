#pragma once

#include <cstdint>

namespace upgl {
enum class ShaderType { Vertex, Fragment };

struct GLHelper {
  static uint32_t ShaderTypeToGL(ShaderType shaderType);
  static void InitializeOpenGL(bool glDebug = true);
};
} // namespace upgl