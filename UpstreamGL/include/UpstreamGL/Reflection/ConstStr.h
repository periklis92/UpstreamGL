#pragma once

#include <cstddef>
#include <stdexcept>

namespace upgl {
namespace Reflection {
namespace detail {
struct NameCleaner;

class ConstStr {
  const char *p;
  size_t sz;

public:
  template <size_t N> constexpr ConstStr(const char (&a)[N]) : p(a), sz(N - 1) {}

  constexpr char operator[](std::size_t n) const {
    return n < sz ? p[n] : throw std::out_of_range("ConstStr: Character index out of range.");
  }

  constexpr bool operator==(const ConstStr &other) { return sz == other.sz && _cmp(p, other.p); }

  constexpr const char *c_str() const { return p; }

  constexpr std::size_t size() const { return sz; }

  friend struct NameCleaner;

private:
  constexpr bool _cmp(const char *a, const char *b) {
    return *a == *b && (*a != '\0' || _cmp(a + 1, b + 1));
  }
};
} // namespace detail
} // namespace Reflection
} // namespace upgl