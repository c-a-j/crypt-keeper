# pragma once
#include <string>
#include <optional>
#include <unordered_map>
#include <vector>
#include <array>
#include <tuple>


namespace ck::types {
  using namespace std::string_view_literals;
  struct Vault {
    std::string name;
    std::string key_fpr;
  };
  
  struct Secret {
    std::string path;
    std::string key_fpr;
  };
  
  struct ConfigOverrides {
    std::optional<bool> auto_push;
    std::optional<std::string> directory;
    
    static constexpr auto str_fields() {
      return std::array{
        std::pair{"directory"sv, &ConfigOverrides::directory}
      };
    }
    static constexpr auto bool_fields() {
      return std::array{
        std::pair{"auto_push"sv, &ConfigOverrides::auto_push}
      };
    }
  };
  
  struct Config {
    std::optional<std::string> vault;
    std::optional<std::string> directory;
    std::optional<bool> auto_push = false;
    std::vector<std::string> set_args;
    std::unordered_map<std::string, ConfigOverrides> overrides;
    
    static constexpr auto str_fields() {
      return std::array {
        std::pair{"vault"sv, &Config::vault},
        std::pair{"directory"sv, &Config::directory}
      };
    }
    static constexpr auto bool_fields() {
      return std::array {
        std::pair{"auto_push"sv, &Config::auto_push}
      };
    }
  };
}
