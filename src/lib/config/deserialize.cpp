#include "toml++/toml.hpp"

#include <filesystem>

#include "util/error.hpp"
#include "lib/config/types.hpp"
#include "util/logger/logger.hpp"

#include "../path/path.hpp"
#include "../path/existence.hpp"
#include "./_internal/types.hpp"

namespace {
  using namespace ck::config;
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;
  template <typename Owner>

  void parse_fields(Owner& cfg, const toml::table& tbl) {
    for (const auto& field : Owner::fields()) {
      std::visit([&](auto member){
        using member_ptr_t = decltype(member);
        using value_t = member_value_t<member_ptr_t>;

        if (auto value = tbl[field.key].template value<value_t>()) {
          cfg.*member = *value;
        }
      }, field.member);
    }
  }
}

namespace ck::config {
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;
  
  void Config::deserialize() {
    std::filesystem::path path = ck::path::config_file();
    if (!ck::path::file_exists(path)) {
      core_.home = ck::path::vault_root();
      return;
    }
    
    toml::table cfg_toml;
    try {
      cfg_toml = toml::parse_file(path.string());
    } catch (const toml::parse_error& e) {
      throw Error<ConfigErrc>{InvalidConfigFile, std::string(e.description())};
    }

    for (auto& section : this->sections()) {
      std::visit([&](auto& member) {
        if (auto* tbl = cfg_toml[section.name].as_table()) {
          parse_fields(this->*member, *tbl);
        }
      }, section.member);
    }
  }
}
