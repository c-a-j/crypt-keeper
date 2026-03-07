#pragma once
#include <stdexcept>

namespace ck {
  // Base for all app errors
  struct Error : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  struct ConfigError : Error { using Error::Error; };
  struct VaultError  : Error { using Error::Error; };
  struct CryptoError : Error { using Error::Error; };
}