#pragma once

#include "Shader.h"
#include <utility>

class ShaderBuilder
{
public:
	ShaderBuilder(const char* name = "");
	ShaderBuilder& AddShader(const char* shader, ShaderType shaderType);
	ShaderBuilder& Build();

	operator Shader&&() noexcept { return std::move(m_Shader); }

private:
	static void CheckShaderCompilationError(uint32_t shaderId);
	static void CheckProgramLinkError(uint32_t programId);

private:
	Shader m_Shader;
};