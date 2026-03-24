#pragma once

#include <filesystem>

namespace ck::fs {
  namespace fs = std::filesystem;
  void sync_dir(const fs::path&);
}
