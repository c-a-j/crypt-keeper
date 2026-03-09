#pragma once
#include "lib/types.hpp"

namespace ck::lib::config {
  using namespace ck::types;

  // void load_fields(VaultConfig& obj, const toml::table& tbl);
  void deserialize(Config& cfg);
}
