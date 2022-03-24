#pragma once

#include <map>
#include <string>
#include <tuple>
#include <typeindex>

#include <UpstreamGL/Debug/Debug.h>
#include <UpstreamGL/Reflection/BinarySerializer.h>
#include <UpstreamGL/Reflection/ConstStr.h>

namespace upgl {
namespace Reflection {
namespace detail {
struct NameCleaner {
  static inline constexpr ConstStr clean(ConstStr str) {
    while (*str.p != ':') {
      ++str.p;
      --str.sz;
    }
    str.p += 2;
    str.sz -= 2;
    return str;
  }
};
} // namespace detail

template <class _Class, class _Member> struct MemberNamePair {
  using type = _Member _Class::*;

  detail::ConstStr _name;
  type _ptr;

  constexpr MemberNamePair(detail::ConstStr name, _Member _Class::*ptr) : _name(name), _ptr(ptr) {}
};

struct MemberInfoBase {
  detail::ConstStr Name;
  const std::type_info &_typeinfo, &_mtypeinfo;

  constexpr MemberInfoBase(detail::ConstStr name, const std::type_info &ti,
                           const std::type_info &mti)
      : Name(name), _typeinfo(ti), _mtypeinfo(mti) {}

  MemberInfoBase(const MemberInfoBase &other) = delete;
  MemberInfoBase &operator=(const MemberInfoBase &other) = delete;

  virtual ~MemberInfoBase() = default;
  template <class _Class, class _Member> const _Member &Get(const _Class &obj) const;
  template <class _Class, class _Member> void Set(_Class &obj, const _Member &val);
};

template <class _Class, class _Member> struct MemberInfo : MemberInfoBase {
public:
  constexpr MemberInfo(detail::ConstStr name, _Member _Class::*addr)
      : MemberInfoBase(name, typeid(_Class), typeid(_Member)), _ptr(addr) {}

  constexpr MemberInfo(const MemberNamePair<_Class, _Member> &pair)
      : MemberInfoBase(pair._name, typeid(_Class), typeid(_Member)), _ptr(pair._ptr) {}

  const _Member &Get(const _Class &obj) const { return obj.*_ptr; }
  _Member &Get(_Class &obj) { return obj.*_ptr; }

  void Set(_Class &obj, const _Member &val) { obj.*_ptr = val; }

private:
  _Member _Class::*_ptr;
};

template <class _Class, class _Member> const _Member &MemberInfoBase::Get(const _Class &obj) const {
  UPGL_ASSERT(typeid(_Member) == _mtypeinfo);
  UPGL_ASSERT(typeid(_Class) == _typeinfo);
  return dynamic_cast<const MemberInfo<_Class, _Member> &>(*this).Get(obj);
}

template <class _Class, class _Member> void MemberInfoBase::Set(_Class &obj, const _Member &val) {
  UPGL_ASSERT(typeid(_Member) == _mtypeinfo);
  UPGL_ASSERT(typeid(_Class) == _typeinfo);
  dynamic_cast<MemberInfo<_Class, _Member> &>(*this).Set(obj, val);
}
} // namespace Reflection
} // namespace upgl