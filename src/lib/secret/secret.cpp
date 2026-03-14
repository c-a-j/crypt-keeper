#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "util/logger.hpp"
#include "lib/types.hpp"
#include "lib/crypto/crypto.hpp"
#include "lib/crypto/random.hpp"
#include "lib/secret/secret.hpp"

namespace ck::secret { 
  using namespace index;
  using namespace ck::config;
  using namespace ck::util::logger;
  using namespace ck::crypto;
  namespace fs = std::filesystem;

  std::vector<std::string> parse_path(const Secret& secret) {
    std::vector<std::string> path_parts;
    std::string part;
    std::stringstream ss(secret.path);
    
  
    while (std::getline(ss, part, '/')) {
      if (part == "" || part.empty()) { continue; }
      path_parts.push_back(part);
    }
    
    return(path_parts);
  };
  
  fs::path get_idx_file(const VaultConfig& cfg) {
    fs::path idx = fs::path(*cfg.directory) / fs::path(*cfg.vault) / "idx";
    return(idx);
  }
  
  nlohmann::json serialize_obj(const IndexObj& entry) {
    nlohmann::json j = {
      {"path", entry.path},
      {"uuid", entry.uuid},
    };
    if (entry.key_fpr) {
      j["key_fpr"] = *entry.key_fpr;
    }
    return j;
  }
  
  IndexObj deserialize_obj(const nlohmann::json& j) {
    IndexObj obj;
    obj.path = j["path"];
    obj.uuid = j["uuid"];
    if (j.contains("key_fpr")) obj.key_fpr = j["key_fpr"];
    return obj;
  }
  
  Entry deserialize_entry(const nlohmann::json& j) {
    Entry obj;
    obj.uuid = j["uuid"];
    if (j.contains("key_fpr")) obj.key_fpr = j["key_fpr"];
    return obj;
  }
  
  nlohmann::json deserialize_idx(const VaultConfig& cfg) {
    fs::path idx = get_idx_file(cfg);
    nlohmann::json j = nlohmann::json::array();
    if (!fs::exists(idx)) { return j; }
    std::ifstream in(idx);
    if (!in) { throw std::runtime_error("failed to open index file"); }
    in >> j;
    return j;
  };
  
  IndexObj create_obj(const Secret& secret) {
    IndexObj obj;
    obj.path = parse_path(secret);
    obj.uuid = uuid_v4();
    return obj;
  }
  
  void write_idx(const VaultConfig& cfg, const nlohmann::json& j) {
    fs::path idx = get_idx_file(cfg);
    
    std::ofstream out(idx);
    if (!out) { throw std::runtime_error("failed to open index file"); }
    
    out << j.dump(2) << "\n";
  }
  
  void walk(Node* node, Entry& entry, const std::vector<std::string> path) {
    for (std::size_t i = 0; i < path.size(); ++i) {
      const bool is_last = (i + 1 == path.size());
      
      if (node->entry && !is_last){
        throw std::runtime_error("entry already exists in the path");
      }
      
      node = &node->children[path[i]];
    }
    
    if (node->entry) {
      throw std::runtime_error("duplicate path");
    }
    
    if (!node->children.empty()) {
      throw std::runtime_error("path conflict");
    }
    
    node->entry = std::move(entry);
  }
  
  Index load_index(const nlohmann::json& j) {
    Index idx;
    
    for (auto& obj : j) {
      Entry entry = deserialize_entry(obj);
      std::vector<std::string> path = obj["path"];
      
      Node* node = &idx.root;
      walk(node, entry, path);
    }
    
    return idx;
  }
  
  void insert_entry(Index& idx, const IndexObj& obj) {
    Entry entry;
    entry.uuid = obj.uuid;
    entry.key_fpr = obj.key_fpr;
    std::vector<std::string> path = obj.path;
    Node* node = &idx.root;
    walk(node, entry, path);
  }
  
  void dfs(const Node& node, std::vector<std::string>& path, nlohmann::json& out) {
    if (node.entry) {
      IndexObj obj;
      obj.path = path;
      obj.uuid = node.entry->uuid;
      obj.key_fpr = node.entry->key_fpr;
      out.push_back(serialize_obj(obj));
    }
    
    for (const auto& [key, child] : node.children) {
      path.push_back(key);
      dfs(child, path, out);
      path.pop_back();
    }
  }
  nlohmann::json serialize_index(const Index& idx) {
    nlohmann::json j = nlohmann::json::array();
    std::vector<std::string> path;
    dfs(idx.root, path, j);
    
    return j;
  }
  
  
  void insert(const VaultConfig& cfg, const Secret& secret) {
    nlohmann::json j = deserialize_idx(cfg);
    Index idx = load_index(j);
    IndexObj obj = create_obj(secret);
    insert_entry(idx, obj);
    j = serialize_index(idx);
    write_idx(cfg, j);
  };
}


// Entry find_entry(const Index& idx, const std::string& path){
//   Entry idx_entry;
  
//   return idx_entry;
// }