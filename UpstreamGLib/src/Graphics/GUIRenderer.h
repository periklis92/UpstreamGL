#pragma once

#include <list>

#include "Renderable.h"

class Camera;

// struct RenderQueue
// {
//     void RenderAll();
//     void Add(Renderable* mesh);
//     std::list<Renderable*> m_Meshes;
// };

class GUIRenderer
{
public:
    void Render(Camera* camera);
    void AddToQueue(Renderable* mesh) { m_Meshes.push_back(mesh); }
    void RemoveFromQueue(Renderable* mesh) { m_Meshes.remove(mesh); }
private:
    std::list<Renderable*> m_Meshes;
};