#include <CLI/CLI.hpp>
#include "cmd/init.hpp"
#include "cmd/insert.hpp"
#include "cmd/get.hpp"
#include "cmd/config.hpp"
#include "cli/build_cli.hpp"
#include "lib/types.hpp"
#include "global.hpp"
#include "util/logger.hpp"

namespace ck::cli {
namespace cmd = ck::cmd;
using namespace ck::types;
using namespace ck::util::logger;
  void build_cli(CLI::App& app, Vault& vault, Secret& secret, Config& cfg, int& exit_code) {
    // bool verbose = false;
    
    app.require_subcommand(1, 1);
    
    app.description(std::string(APP_NAME) + " is a command-line tool for managing encrypted secrets.");
    app.footer("More coming soon");
    
    // app.add_option("-v, --verbose", verbose, "Path verbose output");
    
    auto* config = app.add_subcommand("config", "Print and edit config file");
    config -> description("Print and edit config file");
    config -> add_option("args", cfg.set_args, "Key [value]");
    config -> add_option("-v, --vault", vault.name, "Set configs for a specific vault");
    config -> callback([&] { cmd::config::config(vault, cfg); });
    
    auto* init = app.add_subcommand("init", "initialize a new password store");
    init -> add_option("-v,--vault", vault.name, "vault name") -> required();
    init -> add_option("-k,--key", vault.key_fpr, "vault key") -> required();
    init -> callback([&] { exit_code = cmd::init::init_store(vault.name, vault.key_fpr); });
    
    auto* insert = app.add_subcommand("insert", "insert a new secret");
    insert -> add_option("-s,--store", vault.name, "store name");
    insert -> add_option("-k,--key", vault.key_fpr, "encryption key");
    insert -> add_option("path, -p,--path", secret.path, "secret path and name (ex cards/mybank/num") -> required();
    insert -> callback([&] { exit_code = cmd::insert::insert_secret(secret.path, vault.key_fpr); });
  
    auto* get = app.add_subcommand("get", "Get a secret");
    // std::string key;
    // get -> add_option("key", key, "Secret key path") -> required();
    get -> callback([&] { cmd::get::run_get(); });
    
  }
}