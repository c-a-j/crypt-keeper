#include <filesystem>

#include "util/error.hpp"
#include "lib/index/types.hpp"
#include "../path/get_idx_file.hpp"

namespace {
}

namespace ck::index { 
  namespace fs = std::filesystem; 
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;

  Index::Index(const std::string& alias) {}
}
