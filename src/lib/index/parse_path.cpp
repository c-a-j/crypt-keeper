#include <string>
#include <vector>
#include <optional>
#include <sstream>

namespace ck::index { 
  std::optional<std::vector<std::string>> parse_path(const std::optional<std::string> path) {
    if (!path) { return std::nullopt; }
    
    std::string part;
    std::vector<std::string> path_parts;
    std::stringstream ss(*path);
  
    while (std::getline(ss, part, '/')) {
      if (part == "" || part.empty()) { continue; }
      path_parts.push_back(part);
    }
    
    return(path_parts);
  };
}
