#include "Shader.h"
#include <cstdio>
#include <glad/glad.h>

Shader::Shader()
	:m_ProgramId(0)
{
}

Shader::Shader(Shader&& shader) noexcept
	:m_ProgramId(shader.m_ProgramId)
{
	shader.m_ProgramId = 0;
}

Shader& Shader::operator=(Shader&& shader) noexcept
{
	m_ProgramId = shader.m_ProgramId;
	shader.m_ProgramId = 0;
	return *this;
}

Shader::~Shader()
{
	if (m_ProgramId)
		glDeleteProgram(m_ProgramId);
}

void Shader::Bind()
{
	glUseProgram(m_ProgramId);
}

void Shader::SetMat4(const char* name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(m_ProgramId, name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetMat4(const char* name, size_t num, const glm::mat4* matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(m_ProgramId, name), num, GL_FALSE, &(*matrix)[0][0]);
}
