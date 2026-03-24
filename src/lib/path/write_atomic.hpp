#pragma once

#include <filesystem>

namespace ck::path {
  namespace fs = std::filesystem;
  void write_atomic(const fs::path&, const std::string&);
}
