#pragma once

class Camera;
class Material;

class Renderable
{
public:
    virtual ~Renderable() = default;
    
    virtual void Render(Camera* camera) = 0;
    virtual const Material* GetMaterial() const { return m_Material; }

protected:
    Material* m_Material;
};