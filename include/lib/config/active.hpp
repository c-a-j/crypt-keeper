#pragma once

#include "lib/types.hpp"

namespace ck::config {
  VaultConfig get_active_config(const Config&, const Vault&);
}