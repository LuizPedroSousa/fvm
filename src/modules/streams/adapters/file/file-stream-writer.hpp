#pragma once
#include "stream-buffer.hpp"
#include "stream-writer.hpp"
#include <filesystem>
#include <fstream>

namespace astralix {
class FileStreamWriter : public StreamWriter {
public:
  FileStreamWriter(const std::filesystem::path &filename, size_t size);
  FileStreamWriter(const std::filesystem::path &path,
                   Scope<StreamBuffer> buffer);
  ~FileStreamWriter();

  void write() override;
  void flush() override;

private:
  std::ofstream m_file;
  std::filesystem::path m_path;
};

} // namespace astralix
