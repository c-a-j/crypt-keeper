#include <string>
#include <vector>

#include "util/error.hpp"
#include "../path/join.hpp"
#include "lib/index/types.hpp"

namespace {
  std::string along_path_msg(const std::vector<std::string>& path, std::size_t i) {
    std::string msg;
    msg += ck::path::join(path);
    msg += " -> the secret '";
    msg += (i > 0) ? path[i-1] : "";
    msg += "' already exists along this path";
    return msg;
  }
}

namespace ck::index { 
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  Node* walk_path(Node* node, const std::vector<std::string> path) {
    for (std::size_t i = 0; i < path.size(); ++i) {
      if (node->entry){
        throw Error<IndexErrc>{PathConflict, along_path_msg(path, i)};
      }
      
      node = &node->children[path[i]];
    }
    
    if (node->entry) {
      throw Error<IndexErrc>{SecretExists, ck::path::join(path)};
    }
    
    if (!node->children.empty()) {
      throw Error<IndexErrc>{PathConflict, ck::path::join(path) + " -> has children"};
    }
    return node;
  }
}
