#pragma once

#include <string>
#include <optional>
#include <vector>
#include <variant>

namespace ck::cli {
  struct RootArgs {
    bool verbose = false;
    bool colors = true;
    bool debug = false;
  };
  
  struct InitArgs {
    std::string vault_name;
    std::string key_fpr;
    std::optional<std::string> directory;
  };

  struct ConfigArgs {
    std::optional<std::string> vault_name;
    std::vector<std::string> set_args;
  };

  struct InsertArgs {
    std::optional<std::string> vault_name;
    std::string path;
    std::optional<std::string> key_fpr;
    bool pwgen = false;
  };

  struct ShowArgs {
    std::optional<std::string> vault_name;
    std::optional<std::string> path;
  };
  
  using CmdArgs = std::variant<
    std::monostate,
    InitArgs,
    ConfigArgs,
    InsertArgs,
    ShowArgs
  >;
  
  struct CliArgs {
    RootArgs root;
    CmdArgs cmd;
  };
  
  struct Context {
    RootArgs root_args;
  };
}
