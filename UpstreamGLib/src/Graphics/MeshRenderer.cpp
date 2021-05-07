#include "MeshRenderer.h"
#include "glad/glad.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <Components/Camera.h>

void MeshRenderer::Render(Camera* camera)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);
    
    for (auto m: m_Meshes)
    {
        m->Render(camera);
    }
    glDisable(GL_DEPTH_TEST);
}