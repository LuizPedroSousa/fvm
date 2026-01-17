#include "stream-buffer.hpp"
#include <cstddef>
#include <utility>

namespace astralix {
class StreamWriter {
public:
  StreamWriter(Scope<StreamBuffer> buffer) : m_buffer(std::move(buffer)) {}

  ~StreamWriter() {}

  virtual void write() = 0;
  virtual void flush() = 0;

protected:
  Scope<StreamBuffer> m_buffer = nullptr;
};

} // namespace astralix
