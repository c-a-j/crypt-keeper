#include <iostream>

#include "cli/types.hpp"
#include "lib/config/active.hpp"
#include "lib/index/find.hpp"

namespace ck::cmd {
  using namespace ck::index;
  using namespace ck::config;
  using namespace ck::secret;
  
  void show(const ck::cli::ShowArgs& args){
    Config cfg;
    Vault vault;
    vault.name = args.vault_name;
    
    Secret secret;
    secret.path = args.path;
    
    VaultConfig acfg;
    config::get_active_config(cfg, acfg, vault);
    
    find(acfg, secret);
  }
}