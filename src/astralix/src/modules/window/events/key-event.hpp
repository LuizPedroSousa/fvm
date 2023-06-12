#pragma once
#include "functional"
#include "iostream"
#include "key-codes.hpp"
#include "queue"

namespace astralix {

class KeyEvent {
public:
  KeyCode get_key() { return m_key_code; }

protected:
  KeyEvent(const KeyCode key_code) : m_key_code(key_code){};
  KeyCode m_key_code;
};

class KeyPressedEvent : public KeyEvent {
public:
  KeyPressedEvent(const KeyCode key_code) : KeyEvent(key_code){};
};

class KeyReleasedEvent : public KeyEvent {
public:
  KeyReleasedEvent(const KeyCode key_code) : KeyEvent(key_code){};
};

class KeyPressedDispatcher {
public:
  static KeyPressedDispatcher *get();

  void attach(KeyPressedEvent listener, std::function<void()> callback) {
    m_listeners.push_back({listener, callback});
  }

  void dispatch(int i) { m_listeners[i].callback(); }

  struct ListenerD {
    KeyEvent event;
    std::function<void()> callback;
  };

  std::vector<ListenerD> m_listeners;

private:
  KeyPressedDispatcher(){};
  static KeyPressedDispatcher *m_instance;
};

class KeyReleasedDispatcher {
public:
  static KeyReleasedDispatcher *get();

  void attach(KeyReleasedEvent listener, std::function<void()> callback) {
    m_listeners.push_back({listener, callback});
  }

  void dispatch(int i) { m_listeners[i].callback(); }

  struct ListenerD {
    KeyEvent event;
    std::function<void()> callback;
  };

  std::vector<ListenerD> m_listeners;

private:
  KeyReleasedDispatcher(){};
  static KeyReleasedDispatcher *m_instance;
};

} // namespace astralix