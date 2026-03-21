#include "cli/types.hpp"
#include "lib/index/find.hpp"

namespace ck::cmd {
  void show(const ck::cli::Context& ctx, const ck::cli::ShowArgs& args){
    ck::config::Config cfg;
    ck::index::find(acfg, args);
  }
}