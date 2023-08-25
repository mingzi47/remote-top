#pragma once

#include <chrono>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <source_location>

namespace minilog {

#define MINILOG_FOREACH_LEVEL(f) f(debug) f(info) f(warn) f(error)

enum class log_level : u_int8_t {
#define MINILOG_LEVEL_FUNCTION(name) name,
  MINILOG_FOREACH_LEVEL(MINILOG_LEVEL_FUNCTION)
#undef MINILOG_LEVEL_FUNCTION
};

namespace details {
inline auto log_level_to_name(log_level lev) -> std::string {
  switch (lev) {
#define MINILOG_LEVEL_TO_NAME_FUNCTION(name)                                   \
  case log_level::name:                                                        \
    return #name;
    MINILOG_FOREACH_LEVEL(MINILOG_LEVEL_TO_NAME_FUNCTION)
#undef MINILOG_LEVEL_TO_NAME_FUNCTION
  }
  return "unknown";
}

inline auto log_name_to_level(std::string lev) -> log_level {
#define MINILOG_NAME_TO_LEVEL_FUNCTION(name)                                   \
  if (lev == #name)                                                            \
    return log_level::name;
  MINILOG_FOREACH_LEVEL(MINILOG_NAME_TO_LEVEL_FUNCTION)
#undef MINILOG_NAME_TO_LEVEL_FUNCTION
  return log_level::info;
}

inline log_level g_log_level = []() -> log_level {
  if (auto lev = std::getenv("MINILOG_LEVEL"); lev) {
    return log_name_to_level(lev);
  }
  return log_level::info;
}();

inline std::ofstream g_log_file = []() -> std::ofstream {
  if (auto path = std::getenv("MINILOG_FILE"); path) {
    return std::ofstream(path, std::ios_base::app);
  }
  return std::ofstream();
}();

template <typename T> struct with_source_location {
private:
  T inner;
  std::source_location loc;

public:
  template <typename U>
    requires std::constructible_from<T, U>
  consteval with_source_location(
      U &&inner, std::source_location loc = std::source_location::current())
      : inner(std::forward<U>(inner)), loc(std::move(loc)){};
  constexpr auto format() -> T & { return inner; }
  constexpr auto location() -> std::source_location & { return loc; }
};

inline auto output_log(log_level lev, std::string msg,
                       std::source_location const &loc) -> void {
  std::chrono::zoned_time now(std::chrono::current_zone(),
                              std::chrono::high_resolution_clock::now());
  msg = std::format("{} {}:{}:{} [{}] {}\n", now, loc.file_name(), loc.line(),
                    loc.column(), details::log_level_to_name(lev), msg);
  if (lev >= details::g_log_level) {
    std::cout << msg;
  }

  if (details::g_log_file) {
    details::g_log_file << msg;
  }
}

template <typename... Args>
auto generic_log(log_level lev,
                 details::with_source_location<std::format_string<Args...>> fmt,
                 Args &&...args) -> void {
  auto msg = std::vformat(fmt.format().get(), std::make_format_args(args...));
  output_log(lev, std::move(msg), fmt.location());
}
} // namespace details

inline auto set_log_level(std::string lev) {
  details::g_log_level = details::log_name_to_level(lev);
}

inline auto set_log_file(std::string path) {
  details::g_log_file = std::ofstream(path);
}

// generic log api, debug("this is a warn") ....
#define MINILOG_FUNCTION(name)                                                 \
  template <typename... Args>                                                  \
  auto name(details::with_source_location<std::format_string<Args...>> fmt,    \
            Args &&...args)                                                    \
      ->void {                                                                 \
    return generic_log(log_level::name, std::move(fmt),                        \
                       std::forward<Args>(args)...);                           \
  }
MINILOG_FOREACH_LEVEL(MINILOG_FUNCTION)
#undef MINILOG_FUNCTION

} // namespace minilog
