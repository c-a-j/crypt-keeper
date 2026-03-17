#include "./detail.hpp"
#include "cli/types.hpp"
#include "lib/types.hpp"
#include "lib/config/active.hpp"
#include "lib/index/insert_entry.hpp"

namespace ck::cmd {
  using namespace ck::config;
  using namespace ck::secret;
  void insert(const ck::cli::InsertArgs& args) {
    Config cfg;
    Vault vault;
    vault.name = args.vault_name;
    
    Secret secret;
    secret.path = args.path;
    secret.key_fpr = args.key_fpr;
    secret.pwgen = args.pwgen;
    
    VaultConfig acfg;
    config::get_active_config(cfg, acfg, vault);
    // secret::wisper(secret);
    ck::index::insert(acfg, secret);
  }
}
