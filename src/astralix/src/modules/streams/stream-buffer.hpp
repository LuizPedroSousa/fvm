#pragma once
#include "arena.hpp"

#include "assert.hpp"

#include <cstddef>
#include <cstring>

namespace astralix {

struct StreamBuffer {
public:
  StreamBuffer(size_t capacity) : m_arena(capacity) {}

  ~StreamBuffer() {}

  void write(char *src, size_t size) {
    auto block = m_arena.allocate(size);

    block->data = src;

    m_data = block;
  }

  void reset() { m_data = nullptr; }

  char *data() { return static_cast<char *>(m_data->data); }

  size_t size() const { return m_data->size; }

private:
  ElasticArena m_arena;

  ElasticArena::Block *m_data = nullptr;
  ;
};

} // namespace astralix
