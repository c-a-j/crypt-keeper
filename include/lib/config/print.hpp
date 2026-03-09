#pragma once
#include "lib/types.hpp"
#include <string>

// print funcs
namespace ck::lib::config {
  using namespace ck::types;
  
  void print_config(Config& cfg, Vault& vault);
  void print_parameter(Config& cfg, Vault& vault, std::string key);
}
