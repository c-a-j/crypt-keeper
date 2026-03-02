#include <CLI/CLI.hpp>
#include "cmd/init.hpp"
#include "cmd/get.hpp"
#include "cmd/config.hpp"
#include "cli/build_cli.hpp"

namespace ck::cli {
namespace cmd = ck::cmd;
void build_cli(CLI::App& app, Crypt& crypt) {
  std::string store = "vault.ck";
  bool verbose = false;
  
  app.description("Crypt Keeper is a command-line tool for managing encrypted secrets.");
  app.footer("More coming soon");
  
  app.add_option("-s, --store", store, "Path to encrypted store");
  app.add_option("-v, --verbose", verbose, "Path verbose output");
  
  auto* init = app.add_subcommand("init", "initialize a new crypt");
  init -> add_option("-n,--name", crypt.name, "crypt name") -> required();
  init -> add_option("-k,--key", crypt.key_fpr, "crypt key") -> required();
  init -> callback([&] { cmd::init::run_init(crypt.name, crypt.key_fpr); });
 
  auto* get = app.add_subcommand("get", "Get a secret");
  // std::string key;
  // get -> add_option("key", key, "Secret key path") -> required();
  get -> callback([&] { cmd::get::run_get(); });
  
  auto* config = app.add_subcommand("config", "print config file");
  config -> callback([&] { cmd::config::run_config(); });
}
}