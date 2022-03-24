#include <cstdio>
#include <cstdlib>

#include <glad/glad.h>

#include <UpstreamGL/Graphics/GLHelper.h>

namespace upgl {
static void GLAPIENTRY glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                         GLsizei length, const GLchar *message,
                                         const void *userData);

uint32_t GLHelper::ShaderTypeToGL(ShaderType shaderType) {
  switch (shaderType) {
  case ShaderType::Fragment:
    return GL_FRAGMENT_SHADER;
  case ShaderType::Vertex:
    return GL_VERTEX_SHADER;
  default:
    return 0u;
  }
}

void GLHelper::InitializeOpenGL(bool glDebug) {
  if (!gladLoadGL()) {
    printf("Failed to initialize OPENGL!");
    exit(-1);
  }

  if (glDebug) {
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(glMessageCallback, 0);
  }
}

static void GLAPIENTRY glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                         GLsizei length, const GLchar *message,
                                         const void *userData) {
  // if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
  fprintf(stderr, "GL MESSAGE: %s type = 0x%x, severity = 0x%x, message = %s\n",
          (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}
} // namespace upgl