#pragma once
#include <string>
#include <optional>

namespace ck::tests::util {
  class ScopedGnupgHome {
    public:
      ScopedGnupgHome();
      ~ScopedGnupgHome();
      const std::string& path() const;
    
    private:
      std::string tmp_home_;
      std::optional<std::string> home_;
  };
  
  std::string generate_tmp_key();
}