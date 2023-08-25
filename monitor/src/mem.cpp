#include <mem.h>

namespace mem {
mem_info mem{};

std::ifstream mem_file;

auto get_mem_info() -> mem_info {
    if (mem_file.is_open()) mem_file.close();

    mem_file.open(global::g_path / "proc/meminfo", std::ios_base::in);

    std::string str{};
    while (mem_file.good()) {
        std::getline(mem_file, str);
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

    mem_file.close();

    return mem;
}
} // namespace mem
