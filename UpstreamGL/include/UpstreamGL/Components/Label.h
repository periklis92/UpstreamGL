#pragma once

#include <UpstreamGL/Components/Component.h>
#include <UpstreamGL/Components/LayoutManager.h>
#include <UpstreamGL/Graphics/Font.h>
#include <UpstreamGL/Graphics/Renderable.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace upgl {
class Node;

struct LabelLayout {
  HorizontalAnchorPoint HLabelAlignment{};
  VerticalAnchorPoint VLabelAlignment{};

  LabelLayout(HorizontalAnchorPoint hanchor = HorizontalAnchorPoint::Left,
              VerticalAnchorPoint vanchor = VerticalAnchorPoint::Middle)
      : HLabelAlignment(hanchor), VLabelAlignment(vanchor) {}
};

class Label : public Component, public Renderable {
private:
  LabelLayout m_LabelLayout{};
  std::string m_Text{};
  const Font *m_Font{nullptr};
  AABB m_AABB{};

public:
  Label(Node *node);

  virtual void OnEnter() override;
  virtual void OnExit() override;

  const std::string GetComponentName() const override { return "Label"; }

  virtual void Render(glm::mat4 viewProj) override;

  void SetText(const char *text);
  void SetFont(Font &font);

  void SetLayout(const LabelLayout &layout) { m_LabelLayout = layout; }
  LabelLayout GetLayout() { return m_LabelLayout; }

  AABB GetBounds() const override { return m_AABB; }
  Node *GetNode() override { return Component::GetNode(); }

private:
  void __CalculateBounds();
};
} // namespace upgl