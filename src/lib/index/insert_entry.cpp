#include <nlohmann/json.hpp>
#include <string>

#include "cli/types.hpp"

#include "util/error.hpp"

#include "lib/types.hpp"
#include "lib/crypto/random.hpp"

#include "./_internal/types.hpp"
#include "./_internal/write_index.hpp"
#include "./_internal/parse_path.hpp"
#include "./_internal/walk_path.hpp"
#include "./_internal/serialize.hpp"
#include "./_internal/deserialize.hpp"

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