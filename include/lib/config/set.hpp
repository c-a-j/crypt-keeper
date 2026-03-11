#pragma once
#include "lib/types.hpp"
#include <string>
#include <vector>

namespace ck::config {
  void set_parameter(Config& cfg, Vault& vault, std::vector<std::string> set_args);
}
