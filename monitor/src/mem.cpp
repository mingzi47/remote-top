#include <mem.h>

namespace mem {
mem_info mem{};

static std::ifstream fread;

auto get_mem_info() -> mem_info  {
  if (fread.is_open())
    fread.close();

  fread.open(global::g_path / "proc/meminfo", std::ios_base::in);

  std::string str{};
  while (fread.good()) {
    std::getline(fread, str);
    if (str.starts_with("MemTotal")) {
      str.erase(0, str.find(":") + 1);
      mem.mem_total = std::stoull(str);
    } else if (str.starts_with("MemFree")) {
      str.erase(0, str.find(":") + 1);
      mem.mem_free = std::stoull(str);

    } else if (str.starts_with("MemAvailable")) {
      str.erase(0, str.find(":") + 1);
      mem.mem_available = std::stoull(str);

    } else if (str.starts_with("Cached")) {
      str.erase(0, str.find(":") + 1);
      mem.mem_cached = std::stoull(str);

    } else if (str.starts_with("SwapTotal")) {
      str.erase(0, str.find(":") + 1);
      mem.mem_swap_total = std::stoull(str);

    } else if (str.starts_with("SwapFree")) {
      str.erase(0, str.find(":") + 1);
      mem.mem_swap_free = std::stoull(str);
    }
  }

  fread.close();

  return mem;
}
} // namespace mem
