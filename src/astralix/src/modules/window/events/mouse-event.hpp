#include "functional"

namespace astralix {

class MouseEvent {
public:
  MouseEvent(double x, double y) : m_x(x), m_y(y){};

  double get_x() { return m_x; }
  double get_y() { return m_y; }

private:
  double m_x;
  double m_y;
};

class MouseDispatcher {
public:
  void dispatch(MouseEvent event);

  void attach(std::function<void(MouseEvent event)> callback);

  static MouseDispatcher *get();

  struct Listener {
    std::function<void(MouseEvent event)> callback;
  };

  std::vector<Listener> m_listeners;

private:
  MouseDispatcher(){};

  static MouseDispatcher *m_instance;
};

} // namespace astralix