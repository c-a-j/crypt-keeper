#pragma once

#include "lib/types.hpp"

namespace ck::insert {
  using namespace ck::config;
  using namespace ck::secret;
  void insert(const VaultConfig&, const Secret&);
}