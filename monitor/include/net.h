#pragma once
#include <cstdint>
#include <vector>

/*
  files
    /proc/net/dev
*/

namespace rtop {
struct net {
  using i64 = int64_t;
  i64 m_upload;
  i64 m_download;
  net operator-(const net &rvalue) {
    return net{.m_upload = m_upload - rvalue.m_upload,
               .m_download = m_download - rvalue.m_download};
  }
};

struct nets {
  std::vector<net> m_value;
};
} // namespace rtop
