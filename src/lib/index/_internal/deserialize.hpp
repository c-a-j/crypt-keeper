#include <nlohmann/json.hpp>

#include "lib/types.hpp"

#include "./types.hpp"

namespace ck::index { 
  Entry deserialize_entry(const nlohmann::json& j);
  Index deserialize(const ck::config::VaultConfig&);
}