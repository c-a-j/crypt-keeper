#include "lib/types.hpp"
#include <iostream>
#include <string>

#include "lib/types.hpp"
#include "lib/config/key_parse.hpp"

// inline constexpr std::string_view GLOBAL_CONFIGS = "global";

namespace ck::lib::config {
  using namespace ck::types;
  // using namespace ck::util::error;

  void print_config_ln(std::string key, std::string value, std::string vault = {}) {
    if (vault.empty()) {
      std::cout << "[global." << key << "] = " << value << "\n";
      return;
    }
    std::cout << "[vaults." << vault << "." << key << "] = " << value << "\n";
  }
  
  void print_str_fields(const VaultConfig& obj, const std::string& vault = {}) {
    for (auto& [key, member] : VaultConfig::str_fields()) {
      if (obj.*member) print_config_ln(std::string(key), *(obj.*member), vault);
    }
  }
  
  void print_bool_fields(const VaultConfig& obj, const std::string& vault = {}) {
    for (auto& [key, member] : VaultConfig::bool_fields()) {
      if (obj.*member) print_config_ln(std::string(key), *(obj.*member) ? "true" : "false", vault);
    }
  }
  
  void print_config(Config& cfg, Vault& vault) {
    if (!vault.name) {
      print_str_fields(cfg.global);
      print_bool_fields(cfg.global);
    }
    
    for (const auto& [v, ov] : cfg.overrides) {
      if (vault.name && v != *vault.name) continue;
      print_str_fields(ov, v);
      print_bool_fields(ov, v);
    }
  }
  
  void print_parameter(Config& cfg, Vault& vault, std::string key) {
    ConfigKey cfg_key;
    cfg_key.key = key;
    cfg_key.vault = vault.name;
    parse_key(cfg_key);
    if (cfg_key.scope == "global") {
      VaultConfig& obj = cfg.global;
      for (auto& [k, member] : VaultConfig::str_fields()) {
        if (k == cfg_key.field) {
          if (obj.*member) print_config_ln(std::string(k), *(obj.*member));
          return;
        }
      }
      for (auto& [k, member] : VaultConfig::bool_fields()) {
        if (k == cfg_key.field) {
          if (obj.*member) print_config_ln(std::string(k), *(obj.*member) ? "true" : "false");
          return;
        }
      }
    }
    
    if (cfg_key.scope == "vaults") {
      for (auto& [v, ov] : cfg.overrides) {
        if (v != cfg_key.vault) continue;
        for (auto& [k, member] : VaultConfig::str_fields()) {
          if (k == cfg_key.field) { 
            if (ov.*member) print_config_ln(std::string(k), *(ov.*member), *cfg_key.vault);
            return; 
          }
        }
        for (auto& [k, member] : VaultConfig::bool_fields()) {
          if (k == cfg_key.field) { 
            if (ov.*member) print_config_ln(std::string(k), *(ov.*member) ? "true" : "false", *cfg_key.vault);
            return; 
          }
        }
      }
    }
  }
}
