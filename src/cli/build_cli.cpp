#include <CLI/CLI.hpp>
#include "commands/init.hpp"
#include "commands/get.hpp"
#include "commands/config.hpp"
#include "cli/build_cli.hpp"


void build_cli(CLI::App& app, Crypt crypt) {
  std::string store = "vault.ck";
  bool verbose = false;
  
  app.description("Crypt Keeper is a command-line tool for managing encrypted secrets.");
  app.footer("More coming soon");
  
  app.add_option("-s, --store", store, "Path to encrypted store");
  app.add_option("-v, --verbose", verbose, "Path verbose output");
  
  auto* init = app.add_subcommand("init", "initialize a new crypt");
  init -> add_option("-n,--name", crypt.name, "crypt name") -> required();
  init -> add_option("-k,--key", crypt.key, "crypt key") -> required();
  init -> callback([&] { run_init(crypt.name, crypt.key); });
 
  auto* get = app.add_subcommand("get", "Get a secret");
  // std::string key;
  // get -> add_option("key", key, "Secret key path") -> required();
  get -> callback([&] { run_get(); });
  
  auto* config = app.add_subcommand("config", "print config file");
  config -> callback([&] { run_config(); });
}