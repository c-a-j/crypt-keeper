#include "util/logger/logger.hpp"
#include "cli/types.hpp"
#include "lib/mount/types.hpp"

namespace ck::cmd {
  using ck::util::logger::logger;
  using ck::mount::mnt;
  
  void show(const ck::cli::Context& _, const ck::cli::ShowArgs& args){
    // resolve the path
    // only generate the index needed

    // if printing from root vault, generate an index for every mount
    // root nodes will need to be inserted in their alias locations for printing
    if (!args.path) {
      logger.info("Printing the whole tree");
    } else {
      logger.info("Printing a subtree or secret");
    }
  }
}
