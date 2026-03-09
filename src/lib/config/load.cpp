#include <string>
#include <filesystem>
#include "toml++/toml.hpp"

#include "lib/config/path.hpp"
#include "lib/types.hpp"
#include "util/logger.hpp"

inline constexpr std::string_view GLOBAL_CONFIGS = "global";

namespace ck::lib::config {
  using namespace ck::types;
  namespace fs = std::filesystem;
  using namespace ck::util::logger;

  void load_fields(VaultConfig& obj, const toml::table& tbl) {
    for (const auto& field : VaultConfig::fields()) {
      std::visit([&](auto member) {
        using M = decltype(member);
        if constexpr (std::is_same_v<M, VaultConfig::StrMember>) {
          obj.*member = tbl[field.key].value<std::string>();
        } else {
          obj.*member = tbl[field.key].value<bool>();
        }
      }, field.member);
    }
  }
  
  void load_config(Config& cfg) {
    fs::path cfg_file = app_config_file();
    if (!fs::exists(cfg_file)) {
      logger.error("Config file not found: ", std::string(cfg_file));
      return;
    }
    auto cfg_toml = toml::parse_file(std::string(cfg_file));
    
    // parse global configurations
    if (auto* globals = cfg_toml[GLOBAL_CONFIGS].as_table()) {
      load_fields(cfg.global, *globals);
    }
    
    // parse named vault overrides: [vault.any-name]
    if (auto* vaults = cfg_toml["vaults"].as_table()){
      for (auto&& [k,n] : *vaults) {
        auto* v = n.as_table();
        if (!v) continue;
        
        VaultConfig ov{};
        load_fields(ov, *v);
        cfg.overrides[std::string(k.str())] = std::move(ov);
      }
    }
  }
}
