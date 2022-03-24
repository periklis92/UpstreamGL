#include <cstring>

#include <UpstreamGL/Debug/Debug.h>
#include <UpstreamGL/Graphics/Font.h>

namespace upgl {

int CharBoundingBox::GetWidth() const { return X1 - X0; }
int CharBoundingBox::GetHeight() const { return Y1 - Y0; }

Character::Character(int code, float advanceWidth, float leftSideBearing, int x, int y,
                     CharBoundingBox boundingBox)
    : Code(code), AdvanceWidth(advanceWidth), LeftSideBearing(leftSideBearing),
      BoundingBox(boundingBox), UVx(x), UVy(y) {}

Character::Character() : Character(-1, 0, 0, 0, 0, {0, 0, 0, 0}) {}

Font::Font() : Ascent(0), Descent(0), LineGap(0), m_Characters(nullptr), m_NumCharacters(0) {}

Font::Font(int ascent, int descent, int lineGap)
    : Ascent(ascent), Descent(descent), LineGap(lineGap), m_Characters(nullptr),
      m_NumCharacters(0) {}

void Font::GetTextSize(const char *text, float &width, float &height) const {
  width = 0;
  height = 0;
  const char *tmp = text;
  while (*tmp) {
    UPGL_ASSERT(*tmp < m_NumCharacters);
    auto &c = m_Characters[(*tmp)];
    width += c.AdvanceWidth + c.LeftSideBearing;
    height = std::max(height, (float)LineGap);
    ++tmp;
  }
}

void Font::SetCharacterSet(Character *characters, int numCharacters) {
  if (m_Characters != nullptr)
    delete[] m_Characters;
  m_Characters = new Character[numCharacters];
  m_NumCharacters = numCharacters;
  memcpy((void *)m_Characters, (void *)characters, numCharacters * sizeof(Character));
}

const Character &Font::operator[](size_t index) const {
  UPGL_ASSERT(index < m_NumCharacters);
  return m_Characters[index];
}

const Character &Font::operator[](const char index) const {
  UPGL_ASSERT(index < m_NumCharacters);
  return m_Characters[index];
}
} // namespace upgl