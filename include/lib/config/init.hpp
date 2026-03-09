#pragma once
#include "lib/types.hpp"

namespace ck::lib::config {
  using namespace ck::types;
  
  void init_config(Config& cfg, Vault& vault);
}
