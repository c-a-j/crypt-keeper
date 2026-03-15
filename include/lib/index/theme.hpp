#pragma once

#include <string>

namespace ck::index {
  enum class OutputComponent {
    VaultName,
    NodeName,
    EntryName,
    Line
  };
  std::string get_scheme_ansi(OutputComponent);
}