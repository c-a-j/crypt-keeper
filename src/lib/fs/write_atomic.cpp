#include <string>
#include <filesystem>

#include "util/error.hpp"
#include "./msg.hpp"
#include "./tempfile.hpp"
#include "./sync_dir.hpp"

namespace ck::fs {
  namespace fs = std::filesystem;
  using ck::util::error::Error;
  using ck::util::error::FsErrc;
  using enum ck::util::error::FsErrc;

  void write_atomic(const fs::path& target, const std::string& contents) {
    std::error_code ec;
    fs::path parent = target.parent_path();
    if (!parent.empty()) {
      fs::create_directories(parent, ec);
    }
    if (ec) {
      throw Error<FsErrc>{CreateDirectoryFailed, parent.string() + ": " + ec.message()};
    }

    TempFile tmp(target);
    int err = tmp.write(contents);
    if (err) {
      throw Error<FsErrc>{WriteFailed, msg(tmp.path, err)};
    }

    err = tmp.sync();
    if (err) {
      throw Error<FsErrc>{FsyncFailed, msg(tmp.path, err)};
    }

    err = tmp.close();
    if (err) {
      throw Error<FsErrc>{CloseFailed, msg(tmp.path, err)};
    }

    err = tmp.commit();
    if (err) {
      throw Error<FsErrc>{RenameFailed, msg(tmp.path, err) + " -> " + target.string()};
    }
    
    sync_dir(parent);
  }
}
