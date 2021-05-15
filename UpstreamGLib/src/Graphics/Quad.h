#pragma once

#include "MeshData.h"

#include "glm/glm.hpp"

class Quad
{
public:
    Quad(glm::vec2 size, glm::vec2 center = {0.f, 0.f});
    Quad(const Quad&) = delete;
    Quad& operator=(const Quad&) = delete;

    inline void Render() { m_MeshData->Draw(); }

private:
    MeshData* m_MeshData;
};