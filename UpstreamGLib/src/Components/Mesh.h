#pragma once

#include "Component.h"
#include "Graphics/Renderable.h"
#include "Resources/MeshResource.h"

class MeshData;
class Shader;
class Camera;
class Node;
class AnimationClip;

class Mesh final
    : public Component, public Renderable
{
public:
    Mesh(Node* node, MeshResource mesh = {}, Shader* shader = nullptr)
        :Component(node), m_MeshResource(mesh), m_Shader(shader) {}
        
    virtual void Render(Camera* camera) override;
    void UpdateFromAnimation(const AnimationClip& clip, float time);

    virtual void OnEnter() override;
    virtual void OnExit() override {}

    void SetShader(Shader* shader) { m_Shader = shader; }
    void SetMesh(MeshResource mesh) { m_MeshResource = mesh; }

    const Shader* GetShader() const { return m_Shader; }
    const MeshResource GetMesh() const { return m_MeshResource; }

    virtual const std::string GetComponentName() const override { return "Mesh"; }

private:
    MeshResource m_MeshResource;
    Shader* m_Shader;
};