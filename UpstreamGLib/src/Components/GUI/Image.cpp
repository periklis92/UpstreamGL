#include "Image.h"
#include "Graphics/Quad.h"
#include "Graphics/Material.h"

Image::Image(Node* node)
    :Component(node)
{
    m_Quad = new Quad({100, 100}, {0, 0});
}

void Image::Render(Camera* camera)
{
    m_Material->Bind();
    m_Quad->Render();
}