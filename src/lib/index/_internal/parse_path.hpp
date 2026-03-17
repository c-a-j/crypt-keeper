#pragma once
#include <string>
#include <optional>
#include <vector>

namespace ck::index { 
  std::optional<std::vector<std::string>> parse_path(const std::optional<std::string>);
}