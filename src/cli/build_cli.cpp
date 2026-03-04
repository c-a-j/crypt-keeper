#include <CLI/CLI.hpp>
#include "cmd/init.hpp"
#include "cmd/get.hpp"
#include "cmd/config.hpp"
#include "cli/build_cli.hpp"
#include "global.hpp"

namespace ck::cli {
namespace cmd = ck::cmd;
  void build_cli(CLI::App& app, PasswordStore& pstore, int& exit_code) {
    std::string store = "vault.ck";
    bool verbose = false;
    
    app.require_subcommand(0, 1);
    
    app.description(std::string(APP_NAME) + " is a command-line tool for managing encrypted secrets.");
    app.footer("More coming soon");
    
    app.add_option("-s, --store", store, "Path to encrypted store");
    app.add_option("-v, --verbose", verbose, "Path verbose output");
    
    auto* init = app.add_subcommand("init", "initialize a new password store");
    init -> add_option("-n,--name", pstore.name, "store name") -> required();
    init -> add_option("-k,--key", pstore.key_fpr, "store key") -> required();
    init -> callback([&] { exit_code = cmd::init::init_store(pstore.name, pstore.key_fpr); });
  
    auto* get = app.add_subcommand("get", "Get a secret");
    // std::string key;
    // get -> add_option("key", key, "Secret key path") -> required();
    get -> callback([&] { cmd::get::run_get(); });
    
    auto* config = app.add_subcommand("config", "print config file");
    config -> callback([&] { cmd::config::run_config(); });
  }
}