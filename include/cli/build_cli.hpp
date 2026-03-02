#pragma once
#include <CLI/CLI.hpp>
#include <string>

namespace ck::cli {
  struct Crypt {
    std::string name;
    std::string key_fpr;
  };
  
  void build_cli(CLI::App&, Crypt&);
}