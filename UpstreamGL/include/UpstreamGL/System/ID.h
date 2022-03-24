#pragma once

#include <algorithm>
#include <errno.h>
#include <time.h>

namespace upgl {
class ID {
public:
  const char *C_Str() const { return m_ID; }

  std::string Str() const { return m_ID; }

  size_t GetHash() const;
  bool operator==(const ID &other) const;
  bool operator>(const ID &other) const;

  operator const char *() { return m_ID; }

  friend struct ::std::hash<ID>;

private:
  char m_ID[33]{};

public:
  static ID Generate();
  static void SetSeed(unsigned int s);

private:
  ID();

  static unsigned int m_Seed;

public:
  ID(const ID &other);
  ID &operator=(const ID &other);
  ID(ID &&other) noexcept;
  ID &operator=(ID &&other) noexcept;
};
} // namespace upgl

namespace std {
template <> struct hash<::upgl::ID> {
  size_t operator()(const ::upgl::ID &id) const { return id.GetHash(); }
};
} // namespace std