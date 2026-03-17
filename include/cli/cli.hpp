#pragma once
#include <CLI/CLI.hpp>

#include "cli/types.hpp"

namespace ck::cli {
  CommandArgs parse_cli(CLI::App&, int, char**);
}