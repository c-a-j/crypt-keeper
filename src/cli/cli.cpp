#include <CLI/CLI.hpp>
#include "cli/cli.hpp"

#include "cli/types.hpp"

#include "global.hpp"

namespace ck::cli {
  namespace {
    struct Args {
      InitArgs init;
      ConfigArgs config;
      InsertArgs insert;
      ShowArgs show;
    };
    
    void configure_root(CLI::App& app) {
      // bool verbose = false;
      // app.add_option("-v, --verbose", verbose, "Path verbose output");
      // 
      app.require_subcommand(1, 1);
      
      app.description(std::string(APP_NAME) + " is a command-line tool for managing encrypted secrets.");
      app.footer("More coming soon");
    } 
    
    CLI::App* add_init(CLI::App& app, Args& args) {
      auto* init = app.add_subcommand("init", "initialize a new password store");
      init -> add_option("-v,--vault", args.init.vault_name, "vault name") -> required();
      init -> add_option("-d,--directory", args.init.directory, "vault root directory");
      init -> add_option("-k,--key", args.init.key_fpr, "vault key") -> required();
      // init -> callback([&] { init::init(cfg, vault); });
      return init;
    }
      
    CLI::App* add_config(CLI::App& app, Args& args) {
      auto* config = app.add_subcommand("config", "Print and edit config file");
      config -> description("Print and edit config file");
      config -> add_option("args", args.config.set_args, "Key [value]");
      config -> add_option("-v, --vault", args.config.vault_name, "Set configs for a specific vault");
      // config -> callback([&] { config::config(cfg, vault, set_args); });
      return config;
    }
      
      
    CLI::App* add_insert(CLI::App& app, Args& args) {
      auto* insert = app.add_subcommand("insert", "insert a new secret");
      insert -> add_option("-v,--vault", args.insert.vault_name, "vault name");
      insert -> add_option("-k,--key", args.insert.key_fpr, "encryption key");
      insert -> add_flag("--pwgen", args.insert.pwgen, "insert a randomly generated password");
      insert -> add_option("path, -p,--path", args.insert.path, "secret path and name (ex cards/mybank/num") -> required();
      // insert -> callback([&] { insert::insert(cfg, vault, secret); });
      return insert;
    }
    
    CLI::App* add_show(CLI::App& app, Args& args) {
      auto* show = app.add_subcommand("show", "Show a secret");
      show -> add_option("path, -p, --path", args.show.path, "Secret path");
      show -> add_option("-v,--vault", args.show.vault_name, "vault name");
      // show -> callback([&] { show::show(cfg, vault, secret); });
      return show;
    }
  }
  
  CommandArgs parse_cli(CLI::App& app, int argc, char** argv) {
    Args args = {};
    configure_root(app);
    auto* init = add_init(app, args);
    auto* config = add_config(app, args);
    auto* insert = add_insert(app, args);
    auto* show = add_show(app, args);
    
    if (argc == 1) {
      std::cout << app.help() << '\n';
      return std::monostate{};
    }
    
    app.parse(argc, argv);
    
    if (init->parsed()) { return args.init; }
    if (config->parsed()) { return args.config; }
    if (insert->parsed()) { return args.insert; }
    if (show->parsed()) { return args.show; }
    
    return std::monostate{};
  }
}