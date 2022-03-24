#pragma once

#include <UpstreamGL/Components/Canvas.h>
#include <UpstreamGL/Components/Component.h>
#include <glm/vec2.hpp>

namespace upgl {
enum class HorizontalAnchorPoint { Center, Left, Right };

enum class VerticalAnchorPoint { Middle, Top, Bottom };

struct UILayout {
  HorizontalAnchorPoint HAnchorPoint{};
  VerticalAnchorPoint VAnchorPoint{};
  glm::vec3 Position{0.f, 0.f, 0.f};
  bool AutoResize = true, KeepAspect = true;

  UILayout(HorizontalAnchorPoint hanchor = {}, VerticalAnchorPoint vanchor = {},
           glm::vec3 position = glm::vec3{0.f}, bool autoResize = true, bool keepAspect = true)
      : HAnchorPoint(hanchor), VAnchorPoint(vanchor), Position(position), AutoResize(autoResize),
        KeepAspect(keepAspect) {}
};

class LayoutManager : public Component {
public:
  LayoutManager(Node *node);
  virtual ~LayoutManager() = default;

  void OnEnter() override;
  void OnExit() override;

  void SetLayout(UILayout layout) { m_Layout = layout; }
  const UILayout &GetLayout() const { return m_Layout; }

  const std::string GetComponentName() const override { return "LayoutManager"; }

private:
  bool __Recalculate(const CanvasRecalculateEvent *e);

  UILayout m_Layout{};
};
} // namespace upgl