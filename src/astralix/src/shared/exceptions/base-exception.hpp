#pragma once
#include "exception"
#include "string"

namespace astralix {

class BaseException : public std::exception {
public:
  BaseException(const char *file, int line, std::string message);

  const char *what() const noexcept override {
    return m_formatted_message.c_str();
  }

private:
  std::string m_message;
  int m_line;
  const char *m_file;

  std::string m_formatted_message;
};

} // namespace astralix
