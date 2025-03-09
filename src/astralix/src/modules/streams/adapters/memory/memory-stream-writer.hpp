#include "stream-buffer.hpp"
#include "stream-writer.hpp"

namespace astralix {
class MemoryStreamWriter : public StreamWriter {
public:
  MemoryStreamWriter() : StreamWriter(create_scope<StreamBuffer>(1024)) {}

  void flush() override;
};

} // namespace astralix
