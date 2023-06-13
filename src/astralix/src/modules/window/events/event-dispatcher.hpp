#pragma once
#include "base.hpp"
#include "functional"
#include "key-codes.hpp"
#include "listener.hpp"
#include "vector"

namespace astralix {

class EventDispatcher {
public:
  static void init();

  static EventDispatcher *get();

  // L -> listener
  // E -> an event
  // F -> callback function deduced by the compiler
  template <typename L, typename E, typename F>
  void attach(KeyCode key_code, const F &callback) {
    Scope<L> listener = create_scope<L>(new E(key_code), callback);
    m_listeners.push_back(std::move(listener));
  }

  // overloading for mouse events
  template <typename L, typename E, typename F> void attach(const F &callback) {
    Scope<BaseListener> listener = create_scope<L>(new E(), callback);
    m_listeners.push_back(std::move(listener));
  }

  template <typename L, typename E> void for_each(std::function<void(L &)> fn) {
    for (auto &listener : m_listeners) {
      auto has_event =
          listener->get_event()->get_event_type() == E::get_static_type();

      auto has_listener = listener->get_listener_type() == L::get_static_type();

      if (has_listener && has_event) {
        fn(dynamic_cast<L &>(*listener.get()));
      }
    };
  }

  std::vector<Scope<BaseListener>> m_listeners;

private:
  EventDispatcher(){};
  static EventDispatcher *m_instance;
};

} // namespace astralix