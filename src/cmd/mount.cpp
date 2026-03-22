#include "util/logger/logger.hpp"
#include "cli/types.hpp"
#include "lib/mount/types.hpp"

namespace ck::cmd {
  using ck::util::logger::logger;
  void mount(const ck::cli::Context& ctx, const ck::cli::MountArgs& args) {
    ck::mount::Mounts mnt;
    logger.info("Mounting a vault");
  };
}
