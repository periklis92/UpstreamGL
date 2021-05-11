#pragma once
#include <cstdint>
enum class ShaderType
{
	Vertex, Fragment
};

struct GLHelper
{
	static uint32_t ShaderTypeToGL(ShaderType shaderType);
	static void InitializeOpenGL(bool glDebug = true);
};