#include <monitor/cpu.h>

namespace cpu {

using namespace std::string_literals;

// proc/cpuinfo, proc/stat
std::ifstream cpu_file{};

// "totals"s, "idles"s, "user"s,    "nice"s,        "system"s, "idle"s,
// "iowait"s, "irq"s,   "softirq"s, "stealstolen"s, "guest"s,

std::array<u64, 11> cur_times{};

std::vector<u64> old_idles{};

cpu_info cpu{};

auto get_cpu_info() -> cpu_info & {
    cpu.clear();

    // 获得 cpu 名字，核心数，Hz
    if (cpu_file.is_open()) cpu_file.close();

    cpu_file.open(global::g_path / "proc/cpuinfo", std::ios_base::in);

    if (not cpu_file.good()) {
        minilog::error("failed open proc/cpuinfo");
        exit(1);
    }

    std::string str{};
    bool flag0{true}, flag1{true};
    while (cpu_file.good()) {
        std::getline(cpu_file, str);
        if (str.starts_with("model name") and flag0) {
            cpu.cpu_name = str.substr(str.find(":") + 1);
            flag0 = false;
        } else if (str.starts_with("cpu MHz") and flag1) {
            cpu.cpu_hz = std::stold(str.substr(str.find(":") + 1)) / 1024;
            flag1 = false;
        } else if (str.starts_with("processor")) {
            ++cpu.core_num;
        }
    }

    if (cpu.core_num != cpu.cpu_s.size()) {
        cpu.cpu_s.resize(cpu.core_num + 1, 0);
    }

    if (cpu_file.is_open()) cpu_file.close();
    cpu_file.open(global::g_path / "proc/stat");

    if (not cpu_file.good()) { throw std::exception(); }

    // 计算 cpu 使用率
    str = "";
    if (cpu.cpu_s.size() != old_totals.size()) {
        old_totals.clear();
        old_idles.clear();
        old_totals.resize(cpu.core_num, 0);
        old_idles.resize(cpu.core_num, 0);
    }
    for (int i = 0; i <= cpu.core_num and std::getline(cpu_file, str).good();
         ++i) {
        if (not str.starts_with("cpu")) break;
        str.erase(0, str.find_first_of(" "));
        str.erase(0, str.find_first_not_of(" "));
        for (size_t index = 2; auto c : str) {
            if (c >= '0' and c <= '9') {
                cur_times[index] *= 10;
                cur_times[index] += u64(c - '0');
            } else {
                ++index;
            }
            if (index >= 11) { break; }
        }
        cur_times[0] =
            std::accumulate(cur_times.begin() + 2, cur_times.end(), 0ull);
        cur_times[1] = cur_times[5] + cur_times[6];

        auto calc_total = cur_times[0] - old_totals[i];
        auto calc_idle = cur_times[1] - old_idles[i];

        cpu.cpu_s[i] =
            (calc_total != 0 ? std::clamp(
                 (u64)std::round(
                     (double)(calc_total - calc_idle) * 100 / calc_total),
                 0ul, 100ul)
                             : 0);

        old_totals[i] = cur_times[0];
        old_idles[i] = cur_times[1];

        for (auto &x : cur_times) x = 0;
    }

    return cpu;
}

cpu_info::cpu_info(const cpu_info &rvalue) {
    this->operator=(rvalue);
}

auto cpu_info::operator=(const cpu_info &rvalue) -> cpu_info & {
    cpu_name = rvalue.cpu_name;
    core_num = rvalue.core_num;
    cpu_hz = rvalue.cpu_hz;
    cpu_s = rvalue.cpu_s;
    return *this;
}

cpu_info::cpu_info(cpu_info &&rvalue) noexcept {
    this->swap(rvalue);
}

auto cpu_info::operator=(cpu_info &&rvalue) noexcept -> cpu_info & {
    this->swap(rvalue);
    return *this;
}

auto cpu_info::clear() -> void {
    this->cpu_name.clear();
    this->core_num = 0;
    this->cpu_hz = 0;
    this->cpu_s.clear();
}

auto cpu_info::swap(cpu_info &rvalue) noexcept -> void {
    this->cpu_name = std::move(rvalue.cpu_name);
    this->core_num = rvalue.core_num;
    this->cpu_hz = rvalue.cpu_hz;
    this->cpu_s = std::move(rvalue.cpu_s);
}

} // namespace cpu
