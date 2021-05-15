#include "Components/Transform.h"

class RectTransform
    : public Transform
{
public:
    RectTransform(Node* node)
        :Transform(node) {}

    void SetWidth(float x) { m_Size.x = x; }
    void SetHeight(float y) { m_Size.y = y; }

    float GetWidth() { return m_Size.x; }
    float GetHeight() { return m_Size.y; }
    
    virtual const std::string GetComponentName() const override { return "RectTransform"; }

private:
    glm::vec2 m_Size;
};