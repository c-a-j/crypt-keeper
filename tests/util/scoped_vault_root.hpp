#pragma once
#include <string>
#include <optional>

namespace ck::tests::util {
  class ScopedVaultRoot {
    public:
      ScopedVaultRoot(); 
      ~ScopedVaultRoot();
      
    private:
      std::string tmp_root_;
      std::optional<std::string> root_;
  };
}