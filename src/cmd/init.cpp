#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "cmd/init.hpp"
#include "global.hpp"
#include "util/logger.hpp"
#include "lib/crypto.hpp"



namespace ck::cmd::init {
  namespace fs = std::filesystem;
  using namespace ck::util::logger;
    
  static std::string env_or_empty(const char* name) {
    if (const char* value = std::getenv(name)) return value;
    return {};
  }
  
  fs::path crypt_root() {
  #ifdef _WIN32
    auto appdata = env_or_empty("APPDATA");
    if (appdata.empty()) throw std::runtime_error("APPDATA is not set");
    return fs::path(appdata) / APP_NAME;
  #else
    auto xdg_data = env_or_empty("XDG_DATA_HOME");
    if (!xdg_data.empty()) return fs::path(xdg_data);
    
    auto home = env_or_empty("HOME");
    if (home.empty()) throw std::runtime_error("HOME is not set");
    return fs::path(home) / ".local" / "share" / APP_NAME;
  #endif
  }
  
  void init_crypt(std::string crypt_name, std::string key_fpr) {
    if (!ck::lib::crypto::public_key_exists(key_fpr)) {
      logger.error("Public key not found: ", key_fpr);
      return;
    }
    
    fs::path dir = crypt_root() / crypt_name;
    std::error_code ec;
    bool created = fs::create_directories(dir, ec);
    if (ec) {
      logger.error("Failed to create crypt: ", ec.message());
      return;
    }
    
    if (!created) {
      std::ostringstream ss;
      logger.error("Crypt already exists: ", dir.string());
      return;
    } 
    
    const fs::path gpg_id_path = dir / ".gpg-id";
    std::ofstream gpg_id_file(gpg_id_path, std::ios::out | std::ios::trunc);
    if (!gpg_id_file.is_open()) {
      logger.error("Failed to open .gpg-id for writing: ", gpg_id_path.string());
      return;
    }
    
    gpg_id_file << key_fpr << '\n';
    if (!gpg_id_file) {
      logger.error("Failed to write .gpg-id: ", gpg_id_path.string());
      return;
    }
    
    logger.success("Crypt initialized: ", dir.string());
  }
  
  void run_init(std::string crypt_name, std::string crypt_key) {
    init_crypt(crypt_name, crypt_key);
  }
}
