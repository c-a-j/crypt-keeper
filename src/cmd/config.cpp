#include <CLI/CLI.hpp>

#include "./detail.hpp"
#include "cli/types.hpp"

#include "lib/config/deserialize.hpp"
#include "lib/config/print.hpp"
#include "lib/config/set.hpp"
#include "lib/config/save.hpp"


namespace ck::cmd {
  void config(const ck::cli::Context& ctx, const ck::cli::ConfigArgs& args) {
    ck::config::Config cfg;
    deserialize(cfg); 
    
    std::vector<std::string> set_args = args.set_args;
    
    if (set_args.size() == 0) {
      print_config(cfg, args);
      return;
    } else if (set_args.size() == 1) {
      print_parameter(cfg, args);
      return;
    } else if (set_args.size() == 2) {
      set_parameter(cfg, args);
      save_config(cfg);
      return;
    } else if (set_args.size() > 2) {
      throw CLI::ValidationError("config", "too many arguments");
    } 
    return;
  }
}
