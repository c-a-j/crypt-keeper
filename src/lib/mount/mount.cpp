#include <filesystem>

#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "../path/path.hpp"
#include "../path/parse_path.hpp"
#include "../index/_internal/tree.hpp"
#include "lib/mount/types.hpp"
#include "lib/index/types.hpp"
#include "lib/config/types.hpp"
#include "./_internal/hash.hpp"
#include "./_internal/initialized.hpp"

namespace ck::mount {
  using ck::config::cfg;
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::MountErrc;
  using enum ck::util::error::MountErrc;

  void Mounts::mount(const std::string& path, const std::string& alias) {
    fs::path mnt_file = ck::path::mount_file();
    bool mnt_file_exists = fs::exists(mnt_file);

    if (mnt_file_exists) {
      ResolvedPath rp = this->resolve(alias);
      ck::index::Index idx(rp.vault_path);

      if (ck::index::tree::secret_along_path(&idx.root(), rp.relative_path)) {
        throw Error<MountErrc>{SecretAlongAlias, alias};
      }
    }

    std::vector<std::string> alias_parts = ck::path::parse_path(path);
    fs::path vault_path;
    if (alias_parts.size() == 1) {
      vault_path = fs::path(cfg.home()) / fs::path(path);
    } else {
      vault_path = fs::path(path);
    }

    if (!initialized(vault_path)) { 
      throw Error<MountErrc>{VaultNotInitialized, path};
    }

    // if the mount file does not exist and the vault is initialized, the 
    // vault is mounted as the root vault
    // otherwise, the vault is mounted at the supplied alias
    if (!mnt_file_exists) {
      this->state_.root.path = vault_path;
      this->state_.root.hash = hash();
      this->save();
      this->print();
      return;
    }

    if (this->state_.mounts.contains(alias)) {
      throw Error<MountErrc>{AliasExists, alias};
    }

    this->state_.mounts[alias].path = vault_path;
    this->state_.mounts[alias].hash = hash();
    this->save();
    this->print();
  }

  void Mounts::mount(const std::string& alias) {
  // this function provides a quick way to mount a vault that is in cfg.core_.home
  // 'ck mount foo' will attempt to mount a vault on disk at cfg.core_.home/foo
  // supplying an alias comprised of more than one component, ex foo/bar is not supported
    std::vector<std::string> alias_parts = ck::path::parse_path(alias);
    logger.debug("Mounts::mount() - adding a mount with a single argument");
    if (alias_parts.size() > 1) {
      throw Error<MountErrc>{InvalidArguments, "a path must be specified for this alias"};
    }

    fs::path vault_path = fs::path(cfg.home()) / fs::path(alias);
    if (initialized(vault_path)) {
      this->mount(alias, vault_path);
      return;
    }
    throw Error<MountErrc>{VaultNotInitialized, vault_path};
  }
}
