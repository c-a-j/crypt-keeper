#pragma once
#include "lib/types.hpp"

#include <optional>

namespace ck::config {
  void load_default(Config& cfg, const std::optional<std::string>& vault_name = std::nullopt);
}
