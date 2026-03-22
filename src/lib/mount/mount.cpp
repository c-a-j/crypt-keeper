#include "toml++/toml.hpp"
#include <filesystem>

#include "global.hpp"
#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "cli/types.hpp"
#include "../path/path.hpp"
#include "../path/parse_path.hpp"
#include "lib/mount/types.hpp"
#include "lib/config/types.hpp"

namespace fs = std::filesystem;

namespace {
  bool initialized(const fs::path& path) { 
    fs::path gpg_id_file = path / fs::path(GPG_ID_FILE);
    if (fs::exists(gpg_id_file)) {
      return true; 
    }
    return false;
  }
  std::string hash() { return "0123456789"; }
}

namespace ck::mount {
  using ck::config::cfg;
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::MountErrc;
  using enum ck::util::error::MountErrc;

  Mounts::Mounts() {}

  Mount& Mounts::root() {
    return this->root_;
  }
  
  const Mount& Mounts::root() const {
    return this->root_;
  }

  std::unordered_map<std::string, Mount>& Mounts::mounts() {
    return this->mounts_;
  }
  
  const std::unordered_map<std::string, Mount>& Mounts::mounts() const {
    return this->mounts_;
  }

  bool Mounts::empty() const noexcept {
    return (this->root_.path.empty() && this->mounts_.empty());
  }

  void Mounts::mount(const std::string& alias, const std::string& path) {
    std::vector<std::string> alias_parts = ck::path::parse_path(path);
    fs::path vault_path;
    if (alias_parts.size() == 1) {
      vault_path = fs::path(cfg.home()) / fs::path(path);
    } else {
      vault_path = fs::path(path);
    }
    fs::path mnt_file = ck::path::mount_file();
    if (!initialized(vault_path)) { 
      throw Error<MountErrc>{VaultNotInitialized, path};
    }

    // if the mount file does not exist and the vault is initialized, the 
    // vault is mounted as the root vault
    // otherwise, the vault is mounted at the supplied alias
    if (fs::exists(mnt_file)) {
      deserialize();
    } else {
      this->root_.path = vault_path;
      this->root_.hash = hash();
      this->write();
      return;
    }
    if (this->mounts_.contains(alias)) {
      throw Error<MountErrc>{AliasExists, alias};
    }
    this->mounts_[alias].path = vault_path;
    this->mounts_[alias].hash = hash();
    this->write();
  }

  void Mounts::mount(const std::string& alias) {
  // this function provides a quick way to mount a vault that is in cfg.core_.home
  // 'ck mount foo' will attempt to mount a vault on disk at cfg.core_.home/foo
  // supplying an alias comprised of more than one component, ex foo/bar is not supported
    std::vector<std::string> alias_parts = ck::path::parse_path(alias);
    if (alias_parts.size() > 1) {
      throw Error<MountErrc>{InvalidArguments, "a path must be specified for this alias"};
    }

    fs::path vault_path = fs::path(cfg.home()) / fs::path(alias);
    if (initialized(vault_path)) {
      this->mount(alias, vault_path);
    }
  }

  void Mounts::umount(const std::string& alias) {
    fs::path mnt_file = ck::path::mount_file();
    if (fs::exists(mnt_file)) {
      deserialize();
    } else {
      throw Error<MountErrc>{MountFileNotFound, std::string(mnt_file)};
    }
    if (mounts_.contains(alias)) {
      mounts_.erase(alias);
    } else {
      throw Error<MountErrc>{AliasDoesNotExist, alias};
    }
    this->write();
    logger.info(alias + std::string(" has been unmounted"));
  }

  void Mounts::chroot(const std::string& path) {
    fs::path mnt_file = ck::path::mount_file();
    fs::path vault_path = fs::path(path);

    if (!initialized(vault_path)) { 
      throw Error<MountErrc>{VaultNotInitialized, path};
    }

    if (fs::exists(mnt_file)) {
      this->deserialize();
    }
    this->root_.path = vault_path;
    this->root_.hash = hash();
    this->write();
    return;
  }

  Mounts mnt;
}
