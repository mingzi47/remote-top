#include <format>
#include <functional>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <type_traits>

namespace cmdline {
struct option {
  const std::string opt_name;
  const bool flag; // true , has value
  std::function<void(std::string)> callback;
  template <typename T>
    requires std::constructible_from<std::function<void(std::string)>, T>
  constexpr option(const std::string &&opt_name, bool flag, T &&callback)
      : opt_name(std::move(opt_name)), flag(flag),
        callback(std::forward<T>(callback)){};
};

namespace details {
inline int parse_opt(int &index, int argc, char **argv, option &opt) {
  if (argv[index] == opt.opt_name) {
    if (not opt.flag) {
      opt.callback("");
      return 1;
    }
    if (index + 1 >= argc) {
      std::cerr << std::format("missing value to option {}\n", opt.opt_name);
      exit(-1);
    }
    opt.callback(argv[++index]);
    return 1;
  }
  return 0;
}
} // namespace details

template <typename T>
concept Opt = std::is_same_v<T, option>;

template <Opt... Opts> void parse(int argc, char **argv, Opts &&...opts) {
  for (int i = 1; i < argc; ++i) {
    int res = 0;
    // int arr[] = {(++size, details::parse_opt(i, argc, argv, opts))...};
    ((res += details::parse_opt(i, argc, argv, opts)), ...);
    // if (std::accumulate(arr, arr + size, 0) == 0) {
    if (res == 0) {
      std::cerr << std::format("invalid option : {}\n", argv[i]);
      exit(-1);
    }
  }
}
} // namespace cmdline
