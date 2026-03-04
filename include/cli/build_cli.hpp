#pragma once
#include <CLI/CLI.hpp>
#include <string>

namespace ck::cli {
  struct PasswordStore {
    std::string name;
    std::string key_fpr;
  };
  
  void build_cli(CLI::App&, PasswordStore&, int&);
}