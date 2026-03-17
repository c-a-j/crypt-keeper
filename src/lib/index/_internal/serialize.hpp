#include <nlohmann/json.hpp>

#include "./get_idx_file.hpp"
#include "./types.hpp"

namespace ck::index { 
  nlohmann::json serialize_obj(const IndexObj& entry);
  IndexObj create_obj(const std::string&);
  nlohmann::json serialize(const Index&);
}