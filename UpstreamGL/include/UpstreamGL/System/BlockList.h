#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>

namespace upgl {
template <class T, ::std::size_t NumChunks> class BlockList;

template <class T, ::std::size_t NumChunks> class BlockListIterator;

template <class T> struct Chunk {
  T val;
  Chunk *m_NextBlock;
};

template <class T, ::std::size_t NumChunks> class Block {
public:
  Block() {
    m_ObjList = reinterpret_cast<Chunk<T> *>(::operator new(sizeof(Chunk<T>) * NumChunks));
    _init();
  }

  Block(const Block &) = delete;
  Block &operator=(const Block &) = delete;

  ~Block() {
    Chunk<T> *c = m_AllocList;
    while (c) {
      reinterpret_cast<T *>(c)->~T();
      c = c->m_NextBlock;
    }
    ::operator delete(m_ObjList);
  }

  [[nodiscard]] constexpr T *allocate() {
    if (is_full()) {
      if (!m_NextBlock) {
        expand();
      }
      return m_NextBlock->allocate();
    }

    Chunk<T> *tmp = m_FreeList;
    m_FreeList = m_FreeList->m_NextBlock;

    T *obj = &tmp->val;
    tmp->m_NextBlock = m_AllocList;
    m_AllocList = tmp;
    return obj;
  }

  constexpr void deallocate(T *p) {
    if (_belongs(p)) {
      Chunk<T> *c = reinterpret_cast<Chunk<T> *>(p);

      if (c == m_AllocList) {
        m_AllocList = c->m_NextBlock;
      } else {
        Chunk<T> *c2 = m_AllocList;
        while (c2) {
          if (c2->m_NextBlock == c) {
            c2->m_NextBlock = c->m_NextBlock;
            break;
          }
          c2 = c2->m_NextBlock;
        }
      }

      c->m_NextBlock = m_FreeList;
      m_FreeList = c;
    } else {
      assert(m_NextBlock);
      m_NextBlock->deallocate(p);
    }
  }

  bool is_full() { return m_FreeList == nullptr; }
  bool is_empty() { return m_AllocList == nullptr; }

  Block *find_free() {
    Block *b = this;
    while (b && b->is_full())
      b = b->m_NextBlock;
    return b;
  }

  void clear() {
    Block<T, NumChunks> *b = m_NextBlock;

    while (b) {
      Block<T, NumChunks> *n = b->m_NextBlock;
      delete b;
      b = n;
    }
    _init();
  }

  void expand() {
    if (m_NextBlock) {
      m_NextBlock->expand();
      return;
    }
    m_NextBlock = new Block();
    assert(m_NextBlock);
  }

private:
  bool _belongs(T *p) {
    assert(p);
    return reinterpret_cast<Chunk<T> *>(p) >= m_ObjList &&
           reinterpret_cast<Chunk<T> *>(p) <= m_ObjList + (sizeof(Chunk<T>) * NumChunks);
  }

  void _init() {
    for (int i = 0; i < NumChunks - 1; ++i) {
      m_ObjList[i].m_NextBlock = &m_ObjList[i + 1];
    }
    m_ObjList[NumChunks - 1].m_NextBlock = nullptr;
    m_FreeList = m_ObjList;
    m_AllocList = nullptr;
    m_NextBlock = nullptr;
  }

private:
  Chunk<T> *m_ObjList;
  Chunk<T> *m_AllocList;
  Chunk<T> *m_FreeList;
  Block *m_NextBlock;

  friend class BlockListIterator<T, NumChunks>;
};

template <class T, ::std::size_t NumChunks> class BlockListIterator {
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T *;
  using reference = T &;

  BlockListIterator(Block<T, NumChunks> *b) : curBlock(b) { curChunk = curBlock->m_AllocList; }

  BlockListIterator(Block<T, NumChunks> *b, Chunk<T> *c) : curBlock(b), curChunk(c) {}

  BlockListIterator &operator++() {
    if (!curChunk->m_NextBlock) {
      curBlock = curBlock->m_NextBlock;
      if (curBlock)
        curChunk = curBlock->m_AllocList;
      else
        curChunk = nullptr;
      return *this;
    }

    curChunk = curChunk->m_NextBlock;
    return *this;
  }

  T *operator->() { return &curChunk->val; }

  T &operator*() { return curChunk->val; }

  bool operator==(const BlockListIterator &other) const {
    return curBlock == other.curBlock && curChunk == other.curChunk;
  }

  bool operator!=(const BlockListIterator &other) const { return !operator==(other); }

private:
  Block<T, NumChunks> *curBlock;
  Chunk<T> *curChunk;
};

/**
 * @brief A linked list of preallocated blocks.
 *
 * @tparam T The type of the allocated object.
 * @tparam NumChunks The number of chunks per block.
 */
template <class T, ::std::size_t NumChunks = 64> class BlockList {
public:
  using iterator = BlockListIterator<T, NumChunks>;

  BlockList() { m_HeadBlock = new Block<T, NumChunks>(); }

  BlockList(const BlockList &) = delete;
  BlockList &operator=(const BlockList &) = delete;

  BlockList(BlockList &&other) { *this = ::std::move(other); }

  BlockList &operator=(BlockList &&other) {
    m_HeadBlock = other.m_HeadBlock;
    m_HeadBlock = nullptr;
    return *this;
  }

  ~BlockList() {
    m_HeadBlock->clear();
    delete m_HeadBlock;
  }

  void push(const T &val) {
    assert(m_HeadBlock);
    T *ptr = m_HeadBlock->allocate();
    assert(ptr);
    new (ptr) T(val);
  }

  void push(T &&val) {
    assert(m_HeadBlock);
    T *ptr = m_HeadBlock->allocate();
    assert(ptr);
    new (ptr) T(::std::move(val));
  }

  T &emplace(T &&val) {
    assert(m_HeadBlock);
    T *ptr = m_HeadBlock->allocate();
    assert(ptr);
    new (ptr) T(::std::move(val));
    return *ptr;
  }

  void remove(T *val) {
    assert(val);
    val->~T();
    assert(m_HeadBlock);
    m_HeadBlock->deallocate(val);
  }

  iterator remove(iterator it) {
    it->~T();
    assert(m_HeadBlock);
    auto nextit = ++it;
    m_HeadBlock->deallocate(&(*it));
    return nextit;
  }

  void clear() {
    for (auto &v : *this) {
      v.~T();
    }
    m_HeadBlock->clear();
  }

  BlockListIterator<T, NumChunks> begin() { return BlockListIterator<T, NumChunks>(m_HeadBlock); }

  BlockListIterator<T, NumChunks> end() {
    return BlockListIterator<T, NumChunks>(nullptr, nullptr);
  }

  const BlockListIterator<T, NumChunks> begin() const {
    return BlockListIterator<T, NumChunks>(m_HeadBlock);
  }

  const BlockListIterator<T, NumChunks> end() const {
    return BlockListIterator<T, NumChunks>(nullptr, nullptr);
  }

private:
  Block<T, NumChunks> *m_HeadBlock{nullptr};
};
} // namespace upgl