#include <filesystem>

namespace rtop {

namespace fs = std::filesystem;

class monitor_run {
public:
  monitor_run();
  ~monitor_run();

  void run();

private:
  fs::path m_root_path{"/proc"};
  fs::directory_entry m_root_dir{m_root_path / "root"};

};
}
