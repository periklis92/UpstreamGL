#pragma once

#include <UpstreamGL/Reflection/ConstStr.h>
#include <UpstreamGL/Reflection/MemberInfo.h>
#include <cstddef>
#include <type_traits>

namespace upgl {
namespace Reflection {
template <class _Type, class... _Members> struct TypeInfo {
private:
  detail::ConstStr Name;
  std::tuple<MemberInfo<_Type, _Members>...> Members;

public:
  using type = TypeInfo<_Type, _Members...>;

  constexpr std::size_t NumMembers() const { return sizeof...(_Members); }

  constexpr TypeInfo(detail::ConstStr name, MemberNamePair<_Type, _Members> &&...members)
      : Name(name), Members(std::tuple_cat(std::forward_as_tuple(members)...)) {}

  constexpr const auto &GetMembers() const { return Members; }

  constexpr auto &GetMembers() { return Members; }

  constexpr auto GetMembersArray(MemberInfoBase *(&arr)[sizeof...(_Members)]) {
    return _GetMembersArray(arr);
  }

  template <std::size_t _Index = 0>
      constexpr
      typename std::enable_if < _Index<sizeof...(_Members), void>::type
                                _GetMembersArray(MemberInfoBase *(&m)[sizeof...(_Members)]) {
    m[_Index] = GetMember<_Index>();
    _GetMembersArray<_Index + 1>(m);
  }

  template <std::size_t _Index>
  constexpr typename std::enable_if<_Index >= sizeof...(_Members), void>::type
  _GetMembersArray(MemberInfoBase *(&m)[sizeof...(_Members)]) {
    return;
  }

  template <std::size_t _Index> constexpr MemberInfoBase *GetMember() {
    static_assert(_Index < sizeof...(_Members), "The _Index of the member is out of range");
    return &std::get<_Index>(Members);
  }

  constexpr MemberInfoBase *FindMember(detail::ConstStr name) { return _FindMember(name); }

  template <std::size_t _Index = 0>
      constexpr typename std::enable_if <
      _Index<sizeof...(_Members), MemberInfoBase *>::type _FindMember(detail::ConstStr name) {
    auto m = GetMember<_Index>();
    if (m->Name == name)
      return m;
    else
      return _FindMember<_Index + 1>(name);
  }

  template <size_t _Index = 0>
  constexpr typename std::enable_if<_Index >= sizeof...(_Members), MemberInfoBase *>::type
  _FindMember(detail::ConstStr name) {
    throw std::out_of_range("Reflection: Member is not part of the class!");
    return nullptr;
  }

  template <std::size_t _Index = 0>
      constexpr typename std::enable_if <
      _Index<sizeof...(_Members), void>::type _Serialize(const _Type &obj, std::ostream &os) const {
    std::get<_Index>(Members).Put(obj, os);
    _Serialize<_Index + 1>(obj, os);
  }

  template <size_t _Index>
  constexpr typename std::enable_if<_Index >= sizeof...(_Members), void>::type
  _Serialize(const _Type &obj, std::ostream &os) const {
    os.flush();
    return;
  }

  template <std::size_t _Index = 0>
      constexpr typename std::enable_if <
      _Index<sizeof...(_Members), void>::type _Deserialize(_Type &obj, std::istream &os) {
    std::get<_Index>(Members).Read(obj, os);
    _Deserialize<_Index + 1>(obj, os);
  }

  template <size_t _Index>
  constexpr typename std::enable_if<_Index >= sizeof...(_Members), void>::type
  _Deserialize(_Type &obj, std::istream &os) {
    return;
  }
};

template <class _Type, class... _Members>
constexpr auto CreateTypeInfo(detail::ConstStr name, MemberNamePair<_Type, _Members> &&...members) {
  return TypeInfo<_Type, _Members...>(name,
                                      std::forward<MemberNamePair<_Type, _Members>>(members)...);
}

template <class _Type> constexpr auto CreateTypeInfo(detail::ConstStr name) {
  return TypeInfo<_Type>(name);
}

template <class _T> struct Class {};
template <class _T> struct IsSerializable : std::false_type {};
} // namespace Reflection
} // namespace upgl

#define UPGLR_STR(X) #X
#define UPGLR_CLEAN_NAME(x) upgl::Reflection::detail::namecleaner::clean(UPGL_STR(x));
#define UPGLR_MEMBER(member)                                                                       \
  upgl::Reflection::MemberNamePair {                                                               \
    upgl::Reflection::detail::NameCleaner::clean(UPGLR_STR(member)), &member                       \
  }

#define UPGLR_TYPE_INFO(type)                                                                      \
  namespace upgl {                                                                                 \
  namespace Reflection {                                                                           \
  template <> struct IsSerializable<type> : std::true_type {};                                     \
  template <> struct Class<type> {                                                                 \
    static inline auto TypeInfo = CreateTypeInfo<type>(UPGLR_STR(type));                           \
  };                                                                                               \
  }                                                                                                \
  }

#define UPGLR_TYPE_INFO_MEMBERS(type, ...)                                                         \
  namespace upgl {                                                                                 \
  namespace Reflection {                                                                           \
  template <> struct IsSerializable<type> : std::true_type {};                                     \
  template <> struct Class<type> {                                                                 \
    static inline auto TypeInfo = CreateTypeInfo(UPGLR_STR(type), ##__VA_ARGS__);                  \
  };                                                                                               \
  }                                                                                                \
  }

#define UPGLR_SERIALIZABLE(type)                                                                   \
  namespace upgl {                                                                                 \
  namespace Reflection {                                                                           \
  template <> struct Serializer<type> {                                                            \
    static void Serialize(std::ostream &os, const type &v) {                                       \
      Class<type>::TypeInfo.Serialize(v, os);                                                      \
    }                                                                                              \
                                                                                                   \
    static type Deserialize(std::istream &is) {                                                    \
      type v{};                                                                                    \
      Class<type>::TypeInfo.Deserialize(v, is);                                                    \
      return v;                                                                                    \
    }                                                                                              \
  };                                                                                               \
  }                                                                                                \
  }
