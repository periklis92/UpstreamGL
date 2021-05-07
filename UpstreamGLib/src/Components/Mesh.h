#pragma once

#include "Component.h"
#include "Graphics/Renderable.h"

class MeshData;
class Shader;
class Camera;
class Node;
class AnimationClip;

class Mesh final
    : public Component, public Renderable
{
public:
    Mesh(Node* node, MeshData* meshData = nullptr, Shader* shader = nullptr)
        :Component(node), m_MeshData(meshData), m_Shader(shader) {}
        
    virtual void Render(Camera* camera) override;
    void UpdateFromAnimation(const AnimationClip& clip, float time);

    virtual void OnEnter() override;
    virtual void OnExit() override {}

    void SetShader(Shader* shader) { m_Shader = shader; }
    void SetMesh(MeshData* mesh) { m_MeshData = mesh; }

    const Shader* GetShader() const { return m_Shader; }
    const MeshData* GetMesh() const { return m_MeshData; }

    virtual const std::string GetComponentName() const override { return "Mesh"; }

private:
    MeshData* m_MeshData;
    Shader* m_Shader;
};