#include <CLI/CLI.hpp>

#include "./detail.hpp"
#include "cli/types.hpp"

#include "lib/config/deserialize.hpp"
#include "lib/config/print.hpp"
#include "lib/config/set.hpp"
#include "lib/config/save.hpp"


namespace ck::cmd {
  void config(const ck::cli::ConfigArgs& args) {
    ck::config::Config cfg;
    deserialize(cfg); 
    
    std::vector<std::string> set_args = args.set_args;
    ck::config::Vault vault;
    vault.name = args.vault_name;
    
    if (set_args.size() == 0) {
      print_config(cfg, vault);
      return;
    } else if (set_args.size() == 1) {
      print_parameter(cfg, vault, set_args[0]);
      return;
    } else if (set_args.size() == 2) {
      set_parameter(cfg, vault, set_args);
      save_config(cfg);
      return;
    } else if (set_args.size() > 2) {
      throw CLI::ValidationError("config", "too many arguments");
    } 
    return;
  }
}
