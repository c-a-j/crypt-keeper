#include <filesystem>

#include "util/error.hpp"
#include "lib/index/types.hpp"
#include "../path/get_idx_file.hpp"

namespace {
}

namespace ck::index { 
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;

  Index::Index(const std::string& vault_path) { 
    this->deserialize(vault_path); 
  }

  Index::Index(const std::string& alias, const std::string& vault_path) { 
    this->deserialize(vault_path); 
    this->alias_ = alias;
  }

  Index::Index() {}

  Node Index::root() {
    return this->root_;
  }

  const Node Index::root() const {
    return this->root_;
  }
}
