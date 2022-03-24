#pragma once

#include <UpstreamGL/Graphics/Material.h>
#include <UpstreamGL/Graphics/MeshData.h>
#include <UpstreamGL/Graphics/MeshObject.h>
#include <UpstreamGL/Graphics/Texture2D.h>

namespace upgl {
class FontResource;

struct CharBoundingBox {
  int X0, X1;
  int Y0, Y1;

  CharBoundingBox(int x0, int y0, int x1, int y1) : X0(x0), X1(x1), Y0(y0), Y1(y1) {}

  int GetWidth() const;
  int GetHeight() const;
};

struct Character {
  int Code;
  float AdvanceWidth, LeftSideBearing;
  int UVx, UVy;
  CharBoundingBox BoundingBox;

  Character(int code, float advanceWidth, float leftSideBearing, int x, int y,
            CharBoundingBox boundingBox);
  Character();

  IndexedMeshObject MeshData;
};

class Font {
public:
  int Ascent, Descent, LineGap;

  const Character &operator[](size_t index) const;
  const Character &operator[](const char index) const;
  Material &GetMaterial() { return m_Material; }
  const Material &GetMaterial() const { return m_Material; }
  void GetTextSize(const char *text, float &width, float &height) const;

private:
  Font();
  Font(int ascent, int descent, int lineGap);
  void SetCharacterSet(Character *character, int numCharacters);

  friend class FontResource;

private:
  Material m_Material{};
  Character *m_Characters;
  int m_NumCharacters;
};
} // namespace upgl