#include <UpstreamGL/Graphics/Quad.h>

Quad::Quad(glm::vec2 size, glm::vec2 center)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    vertices.insert(vertices.begin(), 
    {
        {
            {center.x - size.x/2.f, center.y - size.y/2.f, 0.f}, {0, 0, -1}, {0.f, 0.f}
        }, {
            {center.x + size.x/2.f, center.y - size.y/2.f, 0.f}, {0, 0, -1}, {1.f, 0.f}
        }, {
            {center.x - size.x/2.f, center.y + size.y/2.f, 0.f}, {0, 0, -1}, {0.f, 1.f}
        }, {
            {center.x + size.x/2.f, center.y + size.y/2.f, 0.f}, {0, 0, -1}, {1.f, 1.f}
        }
    });

    indices.insert(indices.begin(), 
    {
            0, 1, 2, 1, 3, 2
    });

    m_MeshData = new MeshData(vertices, indices, AABB());
}