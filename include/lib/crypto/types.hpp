#pragma once

#include <optional>

namespace ck::crypto {
  struct PwSpec {
    std::optional<std::size_t> length;
    std::optional<std::size_t> nsym;
    std::optional<std::size_t> nnum;
    std::optional<std::size_t> nupp;
    std::optional<std::size_t> nlow;
  };
}