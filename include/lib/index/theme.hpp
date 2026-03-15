#pragma once

#include <string>
#include "util/term.hpp"

namespace ck::index {
  enum class OutputComponent {
    VaultName,
    NodeName,
    EntryName,
    Line
  };
  ck::util::term::TextStyle get_scheme_style(OutputComponent);
  std::string get_scheme_ansi(OutputComponent);
}