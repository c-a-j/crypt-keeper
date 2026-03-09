#pragma once
#include "lib/types.hpp"
#include <string>
#include <vector>

namespace ck::lib::config {
  using namespace ck::types;
  
  void set_parameter(Config& cfg, Vault& vault, std::vector<std::string> set_args);
}
