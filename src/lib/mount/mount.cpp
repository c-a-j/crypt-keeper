#include "toml++/toml.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <optional>

#include "global.hpp"
#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "cli/types.hpp"
#include "../path/path.hpp"
#include "./_internal/mount.hpp"
#include "./_internal/theme.hpp"

namespace fs = std::filesystem;

namespace {
  bool initialized(const fs::path& path) { return true; }
  std::string hash() { return "0123456789"; }

  toml::table create_table(const ck::mount::Mount& m, const std::optional<std::string> alias = std::nullopt) {
    toml::table tbl; 
    tbl.insert_or_assign("path", m.path);
    if (alias) { tbl.insert_or_assign("alias", *alias); }
    tbl.insert_or_assign("hash", m.hash);
    return tbl;
  }

  toml::table serialize(const ck::mount::Mounts& mounts) {
    toml::table tbl; 
    tbl.insert("root", create_table(mounts.root()));

    toml::array mnts;
    for (auto [k, mnt] : mounts.mounts()) {
      mnts.push_back(create_table(mnt, k));
    }
    tbl.insert("mount", mnts);
    return tbl;
  }
}

namespace ck::mount {
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

  void Mounts::deserialize() {
    fs::path path = ck::path::mount_file();
    if (!fs::exists(path)) {
      throw Error<MountErrc>{MountFileNotFound, "Initialize a vault or mount an existing one"};
    }
    auto mount_toml = toml::parse_file(std::string(path));
    if (auto* tbl = mount_toml["root"].as_table()) {
      Mount m{};
      for (const auto& field : Mount::fields()) {
        if (auto v = (*tbl)[field.key].value<std::string>()) {
          m.*(field.member) = *v;
        }
      }
    }

    if (auto* mounts = mount_toml["mount"].as_array()) {
      for (auto&& node : *mounts) {
        auto* tbl = node.as_table();
        if (!tbl) {
          continue;
        }
        Mount m{};

        for (const auto& field : Mount::fields()) {
          if (auto v = (*tbl)[field.key].value<std::string>()) {
            m.*(field.member) = *v;
          }
        }
        auto alias = (*tbl)["alias"].value<std::string>();
        if (!alias) {
          throw Error<MountErrc>{NoAlias, std::string(m.path)};
        }
        this->mounts_[*alias] = m;
      }
    }
  }


  void Mounts::write() {
    ck::path::create_config_dir();
    fs::path mnt_file = ck::path::mount_file();
    bool exists = fs::exists(mnt_file);
    std::ofstream out(mnt_file, std::ios::out | std::ios::trunc);
    toml::table tbl = serialize(*this);
    out << tbl << "\n";
    if (!out) {
      throw Error<MountErrc>{WriteMountFailed, std::string(mnt_file)};
    }
    if (!exists) {
      logger.info("Created new mount file", std::string(mnt_file));
    }
  }

  void Mounts::mount(const std::string& path, const std::string& alias) {
    fs::path mnt_file = ck::path::mount_file();
    fs::path vault_path = fs::path(path);
    if (!initialized(vault_path)) { 
      throw Error<MountErrc>{VaultNotInitialized, path};
    }

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

  void Mounts::mount(const std::string& path) {
    std::string alias = fs::path(path).filename().string();
    this->mount(path, alias);
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

  using ck::util::term::reset;
  using enum OutputComponent;
  void Mounts::print() {
    fs::path mnt_file = ck::path::mount_file();
    if (!fs::exists(mnt_file)) { return; }
    this->deserialize();
    std::cout 
      << get_scheme_ansi(RootMountPath)
      << APP_NAME 
      << reset()
      << get_scheme_ansi(RootMountArrow)
      << " \u2192 "
      << reset()
      << get_scheme_ansi(RootMountPath)
      << "(" << this->root_.path << ")" 
      << reset()
      << "\n";
  }

  void mount(const ck::cli::Context&, const ck::cli::MountArgs&) {
    Mounts m;
    m.mount("foo", "bar");
    m.mount("foo/bar", "foobar");
    m.mount("fooski/bar", "fooski");
    m.mount("fooski/barski", "barski");
    m.umount("fooski");

    m.chroot("new/root");
    m.print();
  }
}
