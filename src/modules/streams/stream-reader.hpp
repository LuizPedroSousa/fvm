#include "stream-buffer.hpp"

namespace astralix {
class StreamReader {
public:
  StreamReader() = default;

  virtual void read() = 0;

  Scope<StreamBuffer> get_buffer() { return std::move(m_buffer); }

protected:
  Scope<StreamBuffer> m_buffer = nullptr;
};

} // namespace astralix
