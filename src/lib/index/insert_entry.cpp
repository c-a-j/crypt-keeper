#include <nlohmann/json.hpp>
#include <string>

#include "cli/types.hpp"

#include "util/error.hpp"

#include "lib/types.hpp"
#include "lib/crypto/random.hpp"

#include "lib/index/types.hpp"
#include "lib/index/deserialize.hpp"
#include "lib/index/walk_path.hpp"
#include "lib/index/parse_path.hpp"
#include "lib/index/serialize.hpp"
#include "lib/index/write_index.hpp"

namespace ck::index { 
  using namespace ck::config;
  using namespace ck::crypto;
  using namespace ck::util::error;
  using enum ck::util::error::IndexErrc;
  
  IndexObj create_obj(const std::string& path) {
    IndexObj obj;
    std::optional<std::vector<std::string>> path_components = parse_path(path);
    if (!path_components) {
      throw Error<IndexErrc>{NoPath};
    }
    obj.path = *path_components;
    obj.uuid = uuid_v4();
    return obj;
  }
  
  void insert_entry(Index& idx, const IndexObj& obj) {
    Entry entry;
    entry.uuid = obj.uuid;
    entry.key_fpr = obj.key_fpr;
    std::vector<std::string> path = obj.path;
    Node* node = &idx.root;
    walk_path(node, entry, path);
  }
  
  void insert(const VaultConfig& vcfg, const ck::cli::InsertArgs& args) {
    Index idx = deserialize(vcfg);
    IndexObj obj = create_obj(args.path);
    insert_entry(idx, obj);
    write_idx(vcfg, serialize(idx));
  }
}