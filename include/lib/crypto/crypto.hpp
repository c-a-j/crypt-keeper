#pragma once
#include <string>

namespace ck::crypto {
  bool public_key_exists(std::string&);
  bool private_key_exists(std::string&);
  void init_gpgme();
}
