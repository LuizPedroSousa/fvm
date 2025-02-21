#pragma once

#include <cstddef>
#include <unordered_map>
#include <vector>
#define RESET "\033[0m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

#ifdef LOG_TO_CONSOLE
#include <iostream>
#endif

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

namespace astralix {

enum class LogLevel { INFO, WARNING, ERROR, DEBUG };

class Logger {
public:
  static Logger &get() {
    static Logger instance;
    return instance;
  }

  struct Log {
    std::string message;
    std::string timestamp;
    LogLevel level;
    const char *caller;
    std::string file;
    int line;
  };

  template <typename... Args>
  void log(LogLevel level, const char *caller, const char *file, int line,
           Args &&...args) {
#ifdef ENABLE_LOGS
    std::ostringstream log_stream;

    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);
    std::tm *local_time = std::localtime(&current_time);
    std::ostringstream timestamp_stream;
    timestamp_stream << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");

    std::string message = build_message(std::forward<Args>(args)...);
    std::size_t msg_hash = std::hash<std::string>{}(message);

    Log *log = nullptr;

    if (m_log_map.find(msg_hash) != m_log_map.end()) {
      log = m_log_map[msg_hash];
      log->timestamp = timestamp_stream.str();
    } else {
      log = new Log{.message = message,
                    .timestamp = timestamp_stream.str(),
                    .level = level,
                    .caller = caller,
                    .file = extract_filename(file),
                    .line = line};
      m_log_map[msg_hash] = log;
    }

    render_log(log);
#endif
  }

  template <typename... Args> std::string build_message(Args &&...args) {
    std::ostringstream message_stream;
    (message_stream << ... << std::forward<Args>(args));
    return message_stream.str();
  }

private:
  std::unordered_map<std::size_t, Log *> m_log_map;

  void render_log(Log *log) {
    std::ostringstream log_stream;

    log_stream << BOLD << "[" << log->timestamp << "] :: ";

    switch (log->level) {
    case LogLevel::INFO:
      log_stream << CYAN << "[INFO] ";
      break;
    case LogLevel::WARNING:
      log_stream << YELLOW << "[WARNING] ";
      break;
    case LogLevel::ERROR:
      log_stream << RED << "[ERROR] ";
      break;
    case LogLevel::DEBUG:
      log_stream << CYAN << "[DEBUG] ";
      break;
    }

    log_stream << RESET << BOLD << "[" << log->file << "::" << log->line << "]";
    log_stream << " [" << log->caller << "]";

    log_stream << RESET << " :: " << log->message << "\n";

#ifdef LOG_TO_CONSOLE
    std::cout << log_stream.str();
#endif
  }

  std::string extract_filename(const char *file) {
    std::string file_path(file);
    size_t pos = file_path.find_last_of("/\\");
    if (pos != std::string::npos) {
      return file_path.substr(pos + 1);
    }
    return file_path;
  }
};

#ifdef ENABLE_LOGS
#define LOG_INFO(...)                                                          \
  Logger::get().log(LogLevel::INFO, __FUNCTION__, __FILE__, __LINE__,          \
                    __VA_ARGS__)
#define LOG_WARN(...)                                                          \
  Logger::get().log(LogLevel::WARNING, __FUNCTION__, __FILE__, __LINE__,       \
                    __VA_ARGS__)
#define LOG_ERROR(...)                                                         \
  Logger::get().log(LogLevel::ERROR, __FUNCTION__, __FILE__, __LINE__,         \
                    __VA_ARGS__)
#define LOG_DEBUG(...)                                                         \
  Logger::get().log(LogLevel::DEBUG, __FUNCTION__, __FILE__, __LINE__,         \
                    __VA_ARGS__)
#else
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_DEBUG(...)
#endif

} // namespace astralix
