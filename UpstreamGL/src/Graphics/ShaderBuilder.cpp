#include <cstdio>

#include <glad/glad.h>

#include <UpstreamGL/Debug/Debug.h>
#include <UpstreamGL/Graphics/ShaderBuilder.h>

namespace upgl {
ShaderBuilder::ShaderBuilder() : m_Shader() { m_Shader.m_ProgramId = glCreateProgram(); }

ShaderBuilder &ShaderBuilder::AddShader(const char *shader, ShaderType shaderType) {
  uint32_t shaderId = glCreateShader(GLHelper::ShaderTypeToGL(shaderType));
  glShaderSource(shaderId, 1, &shader, NULL);
  glCompileShader(shaderId);
  CheckShaderCompilationError(shaderId);
  glAttachShader(m_Shader.m_ProgramId, shaderId);
  glDeleteShader(shaderId);
  return *this;
}

Shader ShaderBuilder::Build() {
  glLinkProgram(m_Shader.m_ProgramId);
  CheckProgramLinkError(m_Shader.m_ProgramId);
  return m_Shader;
}

void ShaderBuilder::CheckShaderCompilationError(uint32_t shaderId) {
  int32_t result = 0;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
  if (!result) {
    int32_t logSize = 0;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
    char *infoLog = new char[logSize];
    glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);
    UPGL_LOG_ERROR("-----------------Shader Compilation Error-----------------\n %s", infoLog);
  }
}

void ShaderBuilder::CheckProgramLinkError(uint32_t programID) {
  int32_t result = 0;
  glGetProgramiv(programID, GL_LINK_STATUS, &result);
  if (!result) {
    int32_t logSize = 0;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logSize);
    char *infoLog = new char[logSize];
    glGetProgramInfoLog(programID, logSize, NULL, infoLog);
    UPGL_LOG_ERROR("-----------------Program Linking Error-----------------\n %s", infoLog);
  }
}
} // namespace upgl