#pragma once
#include <CLI/CLI.hpp>
#include <string>


struct Crypt {
  std::string name;
  std::string key;
};

void build_cli(CLI::App&, Crypt&);