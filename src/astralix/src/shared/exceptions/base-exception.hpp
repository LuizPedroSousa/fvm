#pragma once
#include "exception"
#include "string"
#include <sstream>

namespace astralix {

class BaseException : public std::exception {
public:
  BaseException(const char *file, const char *function, int line,
                std::string message);

  const char *what() const noexcept override {
    return m_formatted_message.c_str();
  }

private:
  std::string m_message;
  int m_line;
  const char *m_file;
  const char *m_function;

  std::string m_formatted_message;
};

template <typename... Args>
std::string build_exception_message(Args &&...args) {
  std::ostringstream oss;
  (oss << ... << std::forward<Args>(args));
  return oss.str();
}

} // namespace astralix
