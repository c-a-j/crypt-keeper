#pragma once
#include <string>

#include "lib/types.hpp"

namespace ck::secret {
  using namespace ck::config;
  void insert(const VaultConfig& cfg, const Secret& secret);
}