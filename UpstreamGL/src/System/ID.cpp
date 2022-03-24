#include <algorithm>
#include <cstring>

#include <UpstreamGL/System/ID.h>

namespace upgl {
const char __validChars[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                             'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                             'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                             'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                             '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-'};

static size_t __fnv_hash64(const char *key, size_t size);

unsigned int ID::m_Seed = 0;

ID::ID() {}

ID::ID(const ID &other) { strcpy(m_ID, other.m_ID); }

ID &ID::operator=(const ID &other) {
  strcpy(m_ID, other.m_ID);
  return *this;
}

ID::ID(ID &&other) noexcept {
  strcpy(m_ID, other.m_ID);
  memset(other.m_ID, 0, 33);
}

ID &ID::operator=(ID &&other) noexcept {
  strcpy(m_ID, other.m_ID);
  memset(other.m_ID, 0, 33);
  return *this;
}

size_t ID::GetHash() const { return __fnv_hash64(m_ID, 32); }

bool ID::operator==(const ID &other) const {
  return GetHash() == other.GetHash() && strcmp(m_ID, other.m_ID) == 0;
}

bool ID::operator>(const ID &other) const { return GetHash() > other.GetHash(); }

ID ID::Generate() {
  ID newId{};
  time_t a = time_t{};
  time(&a);
  static size_t m = 144115187807420415;
  static size_t r = 1;
  r = ((a * r) % m) ^ m_Seed;

  size_t tmp = r;

  for (int i = 0; i < 32; ++i) {
    newId.m_ID[i] = __validChars[tmp % 63];
    tmp = (tmp << 1) ^ m;
  }

  newId.m_ID[32] = '\0';

  return newId;
}

void ID::SetSeed(unsigned int s) { ID::m_Seed = s; }

size_t __fnv_hash64(const char *key, size_t size) {
  static size_t fnv_prime = 1099511628211;
  static size_t fnv_offset = 14695981039346656037U;

  size_t hash = fnv_offset;

  for (int i = 0; i < size; ++i) {
    hash ^= key[i];
    hash *= fnv_prime;
  }

  return hash;
}
} // namespace upgl