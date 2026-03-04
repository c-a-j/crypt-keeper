#pragma once
#include <string>
#include <optional>

namespace ck::tests::util {
  class ScopedStoreRoot {
    public:
      ScopedStoreRoot(); 
      ~ScopedStoreRoot();
      
    private:
      std::string tmp_root_;
      std::optional<std::string> root_;
  };
}