#pragma once

class Camera;
class Material;

class Renderable
{
public:
    virtual ~Renderable() = default;
    
    virtual void Render(Camera* camera) = 0;
    virtual const Material* GetMaterial() const { return m_Material; }
    virtual void SetMaterial(Material* material) { m_Material = material; }

protected:
    Material* m_Material;
};