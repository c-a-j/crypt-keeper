#pragma once
#include <string>
#include <optional>

namespace ck::tests::util {
  class ScopedCryptRoot {
    public:
      ScopedCryptRoot(); 
      ~ScopedCryptRoot();
      
    private:
      std::string tmp_root_;
      std::optional<std::string> root_;
  };
}