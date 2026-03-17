#pragma once

#include <string>
#include <optional>
#include <vector>
#include <variant>

namespace ck::cli {
  struct RootArgs {
    bool verbose = true;
    bool colors = true;
  };
  
  struct InitArgs {
    std::string vault_name;
    std::string directory;
    std::string key_fpr;
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
  
  using CommandArgs =
    std::variant<std::monostate, ConfigArgs, InitArgs, InsertArgs, ShowArgs>;
}
