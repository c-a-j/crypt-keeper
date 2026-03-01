#include <CLI/CLI.hpp>
#include <string>
#include "cli/build_cli.hpp"

int main(int argc, char** argv) {
  Crypt crypt = {};

  CLI::App app{"crypt-keeper"};
  build_cli(app, crypt);
  CLI11_PARSE(app, argc, argv)
  return 0;
}
