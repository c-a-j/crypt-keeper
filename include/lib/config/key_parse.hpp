#pragma once
#include <string>
#include <optional>

namespace ck::lib::config {
  struct ConfigKey {
    std::string key;
    std::string scope;
    std::optional<std::string> vault;
    std::string field;
    
    explicit operator bool() const { return !key.empty(); }
  };

  void parse_key(ConfigKey& key);
}
