#pragma once
#include <string>

#include "lib/types.hpp"

namespace ck::lib::index {
  using namespace ck::types;
  void insert(const VaultConfig& cfg, const Secret& secret);
}