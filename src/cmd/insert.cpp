#include "util/logger/logger.hpp"
#include "cli/types.hpp"
#include "lib/config/types.hpp"
#include "lib/index/types.hpp"
#include "lib/mount/types.hpp"
#include "./_internal.hpp"

namespace ck::cmd {
  using ck::util::logger::logger;
  void insert(const ck::cli::Context& ctx, const ck::cli::InsertArgs& args) {
    ck::config::Config cfg;
    ck::mount::Mounts mnt;
    ck::index::Index idx;
    logger.info("inserting a secret");
  }
}
