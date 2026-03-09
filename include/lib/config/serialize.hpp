#pragma once
#include "toml++/toml.hpp"

#include "lib/types.hpp"

namespace ck::lib::config {
  using namespace ck::types;
  
  // void insert_fields_to_toml(const VaultConfig& obj, toml::table& tbl);
  toml::table serialize(const Config& cfg);
}