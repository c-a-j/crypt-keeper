#include "cli/types.hpp"
#include "./detail.hpp"

namespace ck::cmd {
using namespace ck::cli;

  int run_command(const CommandArgs& cargs) {
    return std::visit([&](const auto& args) -> int {
      using T = std::decay_t<decltype(args)>;
      
      if constexpr (std::is_same_v<T, std::monostate>) {
        return 0;
      } else if constexpr (std::is_same_v<T, InitArgs>) {
        init(args);
      } else if constexpr (std::is_same_v<T, ConfigArgs>) {
        config(args);
      } else if constexpr (std::is_same_v<T, InsertArgs>) {
        insert(args);
      } else if constexpr (std::is_same_v<T, ShowArgs>) {
        show(args);
      }
      return 0;
    }, cargs);
  }
}