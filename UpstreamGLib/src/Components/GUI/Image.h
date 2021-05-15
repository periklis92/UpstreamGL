#include "Components/Component.h"
#include "Graphics/Renderable.h"

class Quad;

class Image
    : public Component, public Renderable
{
public:
    Image(Node* node);
    
    virtual const std::string GetComponentName() const override { return "Image"; }
    
    virtual void Render(Camera* camera) override;

private:
    Quad* m_Quad;
};