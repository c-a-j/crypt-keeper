#pragma once

#include <vector>
#include <string>
#include "lib/types.hpp"

namespace ck::config {
  void config(Config&, Vault&, std::vector<std::string>&);
}