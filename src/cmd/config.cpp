#include <CLI/CLI.hpp>
#include <iostream>

#include "cmd/config.hpp"
#include "util/logger.hpp"
#include "lib/types.hpp"
#include "lib/config.hpp"

namespace ck::cmd::config {
  using namespace ck::util::logger;
  using namespace ck::types;
  using namespace ck::lib::config;
  
  void config(Vault& vault, Config& cfg) {
    load_config(cfg);
    
    if (cfg.set_args.size() == 0) {
      print_config(vault, cfg);
      return;
    } else if (cfg.set_args.size() == 1) {
      print_config(vault, cfg);
      return;
    } else if (cfg.set_args.size() == 2) {
      std::cout << "setting " << cfg.set_args[0] << " to " << cfg.set_args[1] << "\n";
      return;
    } else if (cfg.set_args.size() > 2) {
      throw CLI::ValidationError("config", "too many arguments");
    } 
    return;
  }
}