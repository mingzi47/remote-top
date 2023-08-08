#include <proc.h>

namespace proc {
namespace fs = std::filesystem;

std::ifstream fread{}, uread{};

std::vector<proc_info> procs{};

std::unordered_map<u64, u64> old_proc_time{}, old_cpu_time{};

auto get_user_name(u64 uid) -> std::string {
    std::string tmp_usr_name{}, line_str{};
    uread.open(global::g_path / "etc/passwd", std::ios_base::in);
    while (uread.good()) {
        std::getline(uread, line_str);
        if (line_str.find(":") == std::string::npos) { continue; }
        tmp_usr_name = line_str.substr(0, line_str.find(":"));
        line_str.erase(0, line_str.find(":x") + 3);
        if (uid == stoull(line_str)) {
            uread.close();
            return tmp_usr_name;
        }
    }
    uread.close();
    return "none";
}

auto get_proc_info(u64 core_num) -> std::vector<proc_info> && {
    procs.clear();
    std::unordered_map<u64, u64> tmp_proc_time{}, tmp_cpu_time{};

    std::string str{};
    fs::directory_entry dir{global::g_path / "proc"};
    for (auto &file : fs::directory_iterator(dir)) {
        if (file.status().type() != fs::file_type::directory) continue;
        std::string filename = file.path().filename().c_str();
        if (not std::isdigit(filename[0])) continue;
        if (fread.is_open()) fread.close();

        fread.open(
            global::g_path / "proc" / filename / "status", std::ios_base::in);

        proc_info new_proc{};

        while (fread.good()) {
            std::getline(fread, str);
            if (str.starts_with("Uid")) {
                str.erase(0, str.find(':') + 1);
                auto uid = std::stoull(str);
                new_proc.proc_usr = get_user_name(uid);
            } else if (str.starts_with("VmRSS")) {
                str.erase(0, str.find(':') + 1);
                new_proc.proc_mem = std::stoull(str);
            }
        }

        fread.close();

        fread.open(
            global::g_path / "proc" / filename / "stat", std::ios_base::in);

        // pid 1 cmd 2 state 3 ppid 4 thread 20
        // utime 14 stime 15 cutime 16 cstime 17

        u64 cpu_t = 0;
        while (fread.good()) {
            std::getline(fread, str);
            std::istringstream ist(str);
            for (int index = 1; index <= 20; ++index) {
                switch (index) {
                    case 1: {
                        ist >> new_proc.proc_pid;
                        break;
                    }
                    case 2: {
                        ist >> new_proc.proc_name;
                        break;
                    }
                    case 3: {
                        ist >> new_proc.proc_state;
                        break;
                    }
                    case 14: {
                        ist >> cpu_t;
                        break;
                    }
                    case 15: {
                        u64 tmp{0};
                        ist >> tmp;
                        cpu_t += tmp;
                        break;
                    }
                    case 16: {
                        u64 tmp{0};
                        ist >> tmp;
                        cpu_t += tmp;
                        break;
                    }
                    case 17: {
                        u64 tmp{0};
                        ist >> tmp;
                        cpu_t += tmp;
                        break;
                    }
                    case 20: {
                        ist >> new_proc.proc_thread_num;
                        break;
                    }
                    default:
                        u64 tmp{0};
                        ist >> tmp;
                        break;
                }
            }
        }

        fread.close();

        fread.open(global::g_path / "proc" / "stat", std::ios_base::in);

        u64 cur_cpu_time{0};
        while (fread.good()) {
            std::getline(fread, str);
            if (str.starts_with("cpu")) {
                str.erase(0, str.find(' '));
                std::istringstream ist(str);
                u64 tmp = 0;
                while (ist >> tmp) cur_cpu_time += tmp;
                break;
            }
        }

        fread.close();

        auto calc_proc_time = cpu_t - old_proc_time[new_proc.proc_pid];
        auto calc_cpu_time = cur_cpu_time - old_cpu_time[new_proc.proc_pid];

        new_proc.proc_cpu = std::clamp(
            std::round((double)calc_proc_time * core_num * 100 / calc_cpu_time),
            0.0, 100.0);

        if (new_proc.proc_name == "") continue;
        procs.push_back(std::move(new_proc));
        tmp_proc_time[new_proc.proc_pid] = cpu_t;
        tmp_cpu_time[new_proc.proc_pid] = cur_cpu_time;
    }

    tmp_proc_time.swap(old_proc_time);
    tmp_cpu_time.swap(old_cpu_time);

    return std::move(procs);
}

proc_info::proc_info(const proc_info &rvalue) {
    this->operator=(rvalue);
}

auto proc_info::operator=(const proc_info &rvalue) -> proc_info & {
    this->proc_pid = rvalue.proc_pid;
    this->proc_mem = rvalue.proc_mem;
    this->proc_thread_num = rvalue.proc_thread_num;
    this->proc_cpu = rvalue.proc_cpu;
    this->proc_name = rvalue.proc_name;
    this->proc_usr = rvalue.proc_usr;
    this->proc_state = rvalue.proc_state;
    return *this;
}

proc_info::proc_info(proc_info &&rvalue) noexcept {
    this->swap(rvalue);
};

auto proc_info::operator=(proc_info &&rvalue) noexcept -> proc_info & {
    this->swap(rvalue);
    return *this;
}

auto proc_info::swap(proc_info &rvalue) noexcept -> void {
    this->proc_pid = rvalue.proc_pid;
    this->proc_mem = rvalue.proc_mem;
    this->proc_thread_num = rvalue.proc_thread_num;
    this->proc_cpu = rvalue.proc_cpu;
    this->proc_name = std::move(rvalue.proc_name);
    this->proc_usr = std::move(rvalue.proc_usr);
    this->proc_state = std::move(rvalue.proc_state);
}

} // namespace proc
