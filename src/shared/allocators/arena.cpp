#include "arena.hpp"

namespace astralix {
Str::Str(const char *str, ElasticArena &arena) : m_arena(arena), m_size(0) {

  auto length = std::strlen(str);

  m_size = length;

  if (length <= SSO_CAPACITY) {
    ASTRA_PROFILE_N("Str constructor SSO");
    for (size_t i = 0; i < length; i++) {
      m_sso[i] = str[i];
    }

    m_sso[length] = '\0'; // null-terminator
    m_is_sso = true;
    return;
  }

  auto block = m_arena.allocate(length + 1);

  char *dest = block->data;

  for (size_t i = 0; i < length; ++i) {
    dest[i] = str[i];
  }

  dest[length] = '\0'; // null-terminator

  m_block = block;
}

size_t Str::allocated_memory() {
  if (!m_size)
    return 0;

  if (m_is_sso) {
    return m_size + 1;
  }

  return m_arena.allocated_memory();
}

Str::~Str() {
  if (m_size > 0) {
    release();
  }
}

void Str::release() {
  if (m_is_sso) {
    for (size_t i = 0; i < m_size; i++) {
      m_sso[i] = 0;
    }
  } else if (m_block != nullptr) {
    m_arena.release(m_block);
  }

  m_is_sso = false;
  m_size = 0;
}
} // namespace astralix
