#pragma once

#include "lib/types.hpp"
#include "util/logger.hpp"

namespace ck::cmd::insert {
  using namespace ck::types;
  using namespace ck::util::logger;
  void insert(const VaultConfig&, const Secret&);
}