#pragma once
#include "types.hpp"

namespace ck::lib::config {
using namespace ck::types;
  void load_config(Config&);
  void print_config(Vault&, Config&);
  void set_config(Vault&, Config&);
}
