#pragma once
#include <cstdint>
#include "GLHelper.h"
#include <glm/glm.hpp>

#define GLR_SHADER_MVP_NAME "_ModelViewProjection"
#define GLR_SHADER_BONE_TRANSFORM_NAME "_BoneTransforms"

class ShaderBuilder;

class Shader
{
private:
	uint32_t m_ProgramId;

	friend class ShaderBuilder;

public:
	Shader();
	Shader(Shader&& shader) noexcept;
	Shader& operator=(Shader&& shader) noexcept;
	Shader(const Shader& shader) = delete;
	Shader& operator=(const Shader&) = delete;
	~Shader();

	bool operator==(const Shader& other) { return m_ProgramId == other.m_ProgramId; }
	bool operator==(const Shader* other) { return m_ProgramId == other->m_ProgramId; }

	void Bind();

	void SetMat4(const char* name, const glm::mat4& matrix);
	void SetMat4(const char* name, size_t num, const glm::mat4* matrix);

};