#include <string>
#include <sstream>
#include <vector>


#include "lib/config/key_parse.hpp"
#include "util/error.hpp"

namespace ck::lib::config {
  using namespace ck::util::error;

  void parse_key(ConfigKey& key) {
    std::string part;
    std::vector<std::string> key_parts;
    std::stringstream ss(key.key);
    
    while (std::getline(ss, part, '.')) {
      key_parts.push_back(part);
    }
    
    // when user specifies vault (ex -v vault auto_push true)
    if (key.vault && key_parts.size() == 1) {
      key.scope = "vaults";
      key.field = key_parts[0];
    // when user does not specify vault (ex vaults.vault.auto_push true)
    } else if (!key.vault && key_parts.size() == 3) {
      key.scope = key_parts[0];
      key.vault = key_parts[1];
      key.field = key_parts[2];
    // when user modifies global settings (ex global.auto_push true)
    } else if (!key.vault && key_parts.size() == 2) {
      key.scope = key_parts[0];
      key.field = key_parts[1];
    } else {
      ss.clear();
      ss << "scope = " << key.scope << ", vault = " << key.vault.value_or("") << 
        ", field = " << key.field << "\n";
      throw Error{ConfigErrc::InvalidSetParameter, ss.str()};
    }
  }
}
