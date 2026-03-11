#include "cmd/insert.hpp"
#include "lib/secret/secret.hpp"
#include "lib/types.hpp"

#include <iostream>

namespace ck::insert {
  using namespace ck::config;
  void insert(const VaultConfig& cfg, const Secret& secret) {
    ck::secret::insert(cfg, secret);
  }
}
