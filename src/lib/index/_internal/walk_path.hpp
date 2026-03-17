#include <nlohmann/json.hpp>
#include <string>

#include "./types.hpp"

namespace ck::index { 
  void walk_path(Node*, Entry&, const std::vector<std::string>);
}