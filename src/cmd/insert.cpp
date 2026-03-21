#include "./_internal.hpp"
#include "cli/types.hpp"
#include "lib/config/types.hpp"

namespace ck::cmd {
  void insert(const ck::cli::Context& ctx, const ck::cli::InsertArgs& args) {
    ck::config::Config cfg;
    ck::index::insert(cfg, args);
  }
}
