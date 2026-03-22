#include "util/logger/logger.hpp"
#include "cli/types.hpp"
#include "lib/mount/types.hpp"

namespace ck::cmd {
  using ck::util::logger::logger;
  void umount(const ck::cli::Context&, const ck::cli::UmountArgs&) {
    ck::mount::Mounts mnt; 
    logger.info("Unmounting a vault");
  };
}
