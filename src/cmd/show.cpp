#include "util/logger/logger.hpp"
#include "cli/types.hpp"
#include "lib/index/types.hpp"

namespace ck::cmd {
  using ck::util::logger::logger;
  void show(const ck::cli::Context& ctx, const ck::cli::ShowArgs& args){
    ck::index::Index idx;
    logger.info("Showing secrets");
  }
}
