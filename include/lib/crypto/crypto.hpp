#pragma once
#include <string>

#include "lib/types.hpp"

namespace ck::crypto {
  // std::string get_fingerprint();
  bool public_key_exists(const std::string&);
  bool private_key_exists(const std::string&);
  void init_gpgme();
}
