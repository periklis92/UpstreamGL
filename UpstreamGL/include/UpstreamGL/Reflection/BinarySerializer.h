#pragma once

#include <array>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace upgl {
namespace Reflection {
template <class T> struct Class;

struct BinarySerializer {
public:
  template <class T> void Serialize(std::ostream &os, const T &v) {
    auto &ti = Reflection::Class<T>().TypeInfo;
    auto &members = ti.GetMembers();
    _SerializeClass(os, std::make_index_sequence<ti.NumMembers()>{}, members, v);
  }

  template <class T> void Deserialize(std::istream &is, T &v) {
    auto &ti = Reflection::Class<T>().TypeInfo;
    auto &members = ti.GetMembers();
    _DeserializeClass(is, std::make_index_sequence<ti.NumMembers()>{}, members, v);
  }

private:
  template <class C, class... M> void _SerializeMembers(std::ostream &os, const C &c, M &&...m) {
    (Serialize(os, m.Get(c)), ...);
  }

  template <size_t... Is, class Tuple, class C>
  void _SerializeClass(std::ostream &os, std::index_sequence<Is...>, Tuple &&t, const C &c) {
    _SerializeMembers(os, c, std::get<Is>(std::forward<Tuple>(t))...);
  }

  template <class C, class... M> void _DeserializeMembers(std::istream &is, C &c, M &&...m) {
    (Deserialize(is, m.Get(c)), ...);
  }

  template <size_t... Is, class Tuple, class C>
  void _DeserializeClass(std::istream &is, std::index_sequence<Is...>, Tuple &&t, C &c) {
    _DeserializeMembers(is, c, std::get<Is>(std::forward<Tuple>(t))...);
  }

public:
  template <> void Serialize<char>(std::ostream &os, const char &v) {
    os.write(static_cast<const char *>(static_cast<const void *>(&v)), sizeof(char));
  }

  template <> void Deserialize<char>(std::istream &is, char &i) {
    is.read(static_cast<char *>(static_cast<void *>(&i)), sizeof(char));
  }

  template <> void Serialize<unsigned char>(std::ostream &os, const unsigned char &v) {
    os.write(static_cast<const char *>(static_cast<const void *>(&v)), sizeof(unsigned char));
  }

  template <> void Deserialize<unsigned char>(std::istream &is, unsigned char &i) {
    is.read(static_cast<char *>(static_cast<void *>(&i)), sizeof(unsigned char));
  }

  template <> void Serialize<short>(std::ostream &os, const short &v) {
    os.write(static_cast<const char *>(static_cast<const void *>(&v)), sizeof(short));
  }

  template <> void Deserialize<short>(std::istream &is, short &i) {
    is.read(static_cast<char *>(static_cast<void *>(&i)), sizeof(short));
  }

  template <> void Serialize<unsigned short>(std::ostream &os, const unsigned short &v) {
    os.write(static_cast<const char *>(static_cast<const void *>(&v)), sizeof(unsigned short));
  }

  template <> void Deserialize<unsigned short>(std::istream &is, unsigned short &i) {
    is.read(static_cast<char *>(static_cast<void *>(&i)), sizeof(unsigned short));
  }

  template <> void Serialize<int>(std::ostream &os, const int &v) {
    os.write(static_cast<const char *>(static_cast<const void *>(&v)), sizeof(int));
  }

  template <> void Deserialize<int>(std::istream &is, int &i) {
    is.read(static_cast<char *>(static_cast<void *>(&i)), sizeof(int));
  }

  template <> void Serialize<unsigned>(std::ostream &os, const unsigned &v) {
    os.write(static_cast<const char *>(static_cast<const void *>(&v)), sizeof(unsigned));
  }

  template <> void Deserialize<unsigned>(std::istream &is, unsigned &i) {
    is.read(static_cast<char *>(static_cast<void *>(&i)), sizeof(unsigned));
  }

  template <> void Serialize<long>(std::ostream &os, const long &v) {
    os.write(static_cast<const char *>(static_cast<const void *>(&v)), sizeof(long));
  }

  template <> void Deserialize<long>(std::istream &is, long &i) {
    is.read(static_cast<char *>(static_cast<void *>(&i)), sizeof(long));
  }

  template <> void Serialize<unsigned long>(std::ostream &os, const unsigned long &v) {
    os.write(static_cast<const char *>(static_cast<const void *>(&v)), sizeof(unsigned long));
  }

  template <> void Deserialize<unsigned long>(std::istream &is, unsigned long &i) {
    is.read(static_cast<char *>(static_cast<void *>(&i)), sizeof(unsigned long));
  }

  template <> void Serialize<long long>(std::ostream &os, const long long &v) {
    os.write(static_cast<const char *>(static_cast<const void *>(&v)), sizeof(long long));
  }

  template <> void Deserialize<long long>(std::istream &is, long long &i) {
    is.read(static_cast<char *>(static_cast<void *>(&i)), sizeof(long long));
  }

  template <> void Serialize<unsigned long long>(std::ostream &os, const unsigned long long &v) {
    os.write(static_cast<const char *>(static_cast<const void *>(&v)), sizeof(unsigned long long));
  }

  template <> void Deserialize<unsigned long long>(std::istream &is, unsigned long long &i) {
    is.read(static_cast<char *>(static_cast<void *>(&i)), sizeof(unsigned long long));
  }

  template <> void Serialize<std::string>(std::ostream &os, const std::string &v) {
    std::size_t size = v.size();
    Serialize(os, size);
    os.write(static_cast<const char *>(static_cast<const void *>(v.c_str())), sizeof(char) * size);
  }

  template <> void Deserialize<std::string>(std::istream &is, std::string &ss) {
    std::size_t size;
    Deserialize(is, size);
    ss.resize(size);
    is.read(static_cast<char *>(static_cast<void *>(&ss[0])), sizeof(char) * size);
  }

  template <class _Type, size_t _Size>
  void Serialize(std::ostream &os, const std::array<_Type, _Size> &v) {
    for (int i = 0; i < _Size; ++i) {
      Serialize(os, v[i]);
    }
  }

  template <class _Type, size_t _Size>
  void Deserialize(std::istream &is, std::array<_Type, _Size> &v) {
    for (int i = 0; i < _Size; ++i) {
      Deserialize(is, v[i]);
    }
  }

  template <class _Type, class _Alloc>
  void Serialize(std::ostream &os, const std::vector<_Type, _Alloc> &v) {
    std::size_t size = v.size();
    Serialize(os, size);
    for (int i = 0; i < size; ++i) {
      Serialize(os, v[i]);
    }
  }

  template <class _Type, class _Alloc>
  void Deserialize(std::istream &is, std::vector<_Type, _Alloc> &v) {
    std::size_t size;
    Deserialize(is, size);
    v.resize(size);
    for (int i = 0; i < size; ++i) {
      Deserialize(is, v[i]);
    }
  }

  template <class _Key, class _Type, class _Pr, class _Alloc>
  void Serialize(std::ostream &os, const std::map<_Key, _Type, _Pr, _Alloc> &v) {
    std::size_t size = v.size();
    Serialize(os, size);
    for (auto kv : v) {
      Serialize(os, kv.first);
      Serialize(os, kv.second);
    }
  }

  template <class _Key, class _Type, class _Pr, class _Alloc>
  void Deserialize(std::istream &is, std::map<_Key, _Type, _Pr, _Alloc> &v) {
    std::size_t size;
    Deserialize(is, size);
    for (int i = 0; i < size; ++i) {
      _Key key;
      _Type val;
      Deserialize(is, key);
      Deserialize(is, val);
      v.emplace(key, val);
    }
  }
};
} // namespace Reflection
} // namespace upgl