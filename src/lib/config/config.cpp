#include "toml++/toml.hpp"

#include <filesystem>
#include <sstream>
#include <iostream>

#include "util/error.hpp"
#include "lib/config/types.hpp"
#include "util/logger/logger.hpp"

#include "../fs/atomic_write.hpp"
#include "../path/path.hpp"
#include "../path/existence.hpp"
#include "./_internal/types.hpp"

namespace ck::config {
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;
  
  Config::Config() { deserialize(); }
  
  const Core& Config::core() const { return core_; }
  Core& Config::core() { return core_; }
  
  const Ui& Config::ui() const { return ui_; }
  Ui& Config::ui() { return ui_; }
  
  const Pwgen& Config::pwgen() const { return pwgen_; }
  Pwgen& Config::pwgen() { return pwgen_; }
  
  const std::string& Config::home() const { 
    if (core_.home.empty()) { 
      throw Error<ConfigErrc>{InvalidConfigFile, "core.home must be defined without --path "};
    }
    return core_.home;
  }
  
  std::string& Config::home() { 
    if (core_.home.empty()) { 
      core_.home = ck::path::vault_root();
    }
    return core_.home;
  }

  Config cfg;
}
