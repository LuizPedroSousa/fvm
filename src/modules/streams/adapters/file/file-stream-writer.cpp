#include "file-stream-writer.hpp"
#include "stream-buffer.hpp"
#include <utility>
namespace astralix {

FileStreamWriter::FileStreamWriter(const std::filesystem::path &path,
                                   size_t size)
    : StreamWriter(create_scope<StreamBuffer>(size)), m_path(path) {
  m_file.open(path, std::ios::binary);
}

FileStreamWriter::FileStreamWriter(const std::filesystem::path &path,
                                   Scope<StreamBuffer> buffer)
    : StreamWriter(std::move(buffer)), m_path(path) {
  m_file.open(path, std::ios::binary);
}

void FileStreamWriter::write() {
  ASTRA_ENSURE(!m_file.is_open(),
                  "Canno write serialized" + std::string(m_path))

  m_file.write(m_buffer->data(), m_buffer->size());
};

FileStreamWriter::~FileStreamWriter() {
  flush();
  m_file.close();
};

void FileStreamWriter::flush() { m_buffer->reset(); };

} // namespace astralix
//
