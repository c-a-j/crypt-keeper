#pragma once
#include <string>

#include "lib/types.hpp"

namespace ck::crypto {
  std::string uuid_v4();
  std::string pwgen(PwSpec&);
}
