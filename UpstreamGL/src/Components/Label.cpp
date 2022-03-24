#include <UpstreamGL/Components/Canvas.h>
#include <UpstreamGL/Components/Label.h>
#include <UpstreamGL/Components/Transform.h>
#include <UpstreamGL/Graphics/Renderer.h>
#include <UpstreamGL/Scene/Director.h>
#include <UpstreamGL/Scene/Node.h>

namespace upgl {
Label::Label(Node *node) : Component(node) {}

void Label::OnEnter() {
  Component::OnEnter();
  auto canvas = m_Node->GetComponent<Transform>()->GetParent()->GetNode()->GetComponent<Canvas>();
  if (canvas->GetCanvasSpace() == CanvasSpace::WorldSpace)
    Director::GetInstance()->GetMeshRenderer().AddToQueue(this);
  else
    Director::GetInstance()->GetUIRenderer().AddToQueue(this);
}

void Label::OnExit() {
  Component::OnExit();
  auto canvas = m_Node->GetComponent<Transform>()->GetParent()->GetNode()->GetComponent<Canvas>();
  if (canvas->GetCanvasSpace() == CanvasSpace::WorldSpace)
    Director::GetInstance()->GetMeshRenderer().RemoveFromQueue(this);
  else
    Director::GetInstance()->GetUIRenderer().RemoveFromQueue(this);
}

void Label::Render(glm::mat4 viewProj) {
  auto transform = m_Node->GetTransform();
  glm::vec3 layoutOffset{0.f, 0.f, 0.f};

  switch (m_LabelLayout.HLabelAlignment) {
  case HorizontalAnchorPoint::Left:
    layoutOffset.x = -m_AABB.GetSize().x;
    break;
  case HorizontalAnchorPoint::Right:
    layoutOffset.x = 0.f;
    break;
  default:
    layoutOffset.x = -m_AABB.GetSize().x / 2.f;
    break;
  }

  switch (m_LabelLayout.VLabelAlignment) {
  case VerticalAnchorPoint::Top:
    layoutOffset.y = -m_AABB.GetSize().y;
    break;
  case VerticalAnchorPoint::Bottom:
    layoutOffset.y = 0.f;
    break;
  default:
    layoutOffset.y = -m_AABB.GetSize().y / 2.f;
    break;
  }

  float advancePosition = 0, yPosition = 0;

  for (int i = 0; i < m_Text.size(); ++i) {
    char c = m_Text[i];

    if (c == '\n') {
      advancePosition = 0.f;
      yPosition -= m_Font->Ascent;
      continue;
    }

    Character cc = m_Font->operator[](c);

    if (i > 0)
      advancePosition += cc.LeftSideBearing;

    glm::mat4 tmp = glm::identity<glm::mat4>();
    tmp *= transform->GetWorldTransform();
    tmp = glm::translate(tmp, glm::vec3{advancePosition, yPosition, 0.f});
    tmp = glm::translate(tmp, glm::vec3{0.f, (float)(m_Font->LineGap - cc.BoundingBox.Y1), 0.f});
    tmp = glm::translate(tmp, layoutOffset);

    m_Material->GetShader().SetMat4(UPGL_SHADER_MVP_NAME, viewProj * tmp);
    cc.MeshData.Bind();
    cc.MeshData.Render();
    advancePosition += cc.AdvanceWidth;
  }
}

void Label::SetText(const char *text) {
  m_Text = text;
  if (m_Font)
    __CalculateBounds();
}

void Label::SetFont(Font &font) {
  m_Material = font.GetMaterial().CreateInstance();
  m_Font = &font;
  if (!m_Text.empty())
    __CalculateBounds();
}

void Label::__CalculateBounds() {
  float width = 0, height = 0;
  m_Font->GetTextSize(m_Text.c_str(), width, height);
  m_AABB.Min = {0, 0, 0};
  m_AABB.Max = {width, height, 0};
}
} // namespace upgl