#include <cstdio>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <UpstreamGL/Graphics/Shader.h>

namespace upgl {
Shader::Shader() : m_ProgramId(0) {}

Shader::Shader(const Shader &shader) { *this = shader; }

Shader &Shader::operator=(const Shader &shader) {
  m_ProgramId = shader.m_ProgramId;
  return *this;
}

Shader::Shader(Shader &&shader) noexcept { *this = shader; }

Shader &Shader::operator=(Shader &&shader) noexcept {
  m_ProgramId = shader.m_ProgramId;
  shader.m_ProgramId = 0;
  return *this;
}

void Shader::Delete() {
  if (m_ProgramId) {
    glDeleteProgram(m_ProgramId);
    m_ProgramId = 0;
  }
}

void Shader::Bind() { glUseProgram(m_ProgramId); }

void Shader::SetMat4(const char *name, const glm::mat4 &matrix) {
  glUniformMatrix4fv(glGetUniformLocation(m_ProgramId, name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetMat4(const char *name, const glm::mat4 *matrix, size_t num) {
  glUniformMatrix4fv(glGetUniformLocation(m_ProgramId, name), num, GL_FALSE, &(*matrix)[0][0]);
}

void Shader::SetInt(const char *name, int value) {
  glUniform1i(glGetUniformLocation(m_ProgramId, name), value);
}

void Shader::SetInt(const char *name, int *value, std::size_t count) {
  switch (count) {
  case 1:
    glUniform1iv(glGetUniformLocation(m_ProgramId, name), 1, value);
    break;
  case 2:
    glUniform2iv(glGetUniformLocation(m_ProgramId, name), 1, value);
    break;
  case 3:
    glUniform3iv(glGetUniformLocation(m_ProgramId, name), 1, value);
    break;
  default:
    glUniform4iv(glGetUniformLocation(m_ProgramId, name), 1, value);
    break;
  }
}

void Shader::SetFloat(const char *name, float *value, std::size_t count) {
  switch (count) {
  case 1:
    glUniform1fv(glGetUniformLocation(m_ProgramId, name), 1, (const GLfloat *)value);
    break;
  case 2:
    glUniform2fv(glGetUniformLocation(m_ProgramId, name), 1, (const GLfloat *)value);
    break;
  case 3:
    glUniform3fv(glGetUniformLocation(m_ProgramId, name), 1, (const GLfloat *)value);
    break;
  default:
    // glUniform4fv(glGetUniformLocation(m_ProgramId, name), 1, (const
    // GLfloat*)value);
    glUniform4fv(glGetUniformLocation(m_ProgramId, name), 1, (const GLfloat *)value);
    break;
  }
}
} // namespace upgl