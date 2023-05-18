#include "key-event.hpp"

KeyPressedDispatcher *KeyPressedDispatcher::m_instance = nullptr;
KeyReleasedDispatcher *KeyReleasedDispatcher::m_instance = nullptr;

KeyPressedDispatcher *KeyPressedDispatcher::get() {
  if (m_instance == nullptr) {
    m_instance = new KeyPressedDispatcher;
  }

  return m_instance;
};

KeyReleasedDispatcher *KeyReleasedDispatcher::get() {
  if (m_instance == nullptr) {
    m_instance = new KeyReleasedDispatcher;
  }

  return m_instance;
};