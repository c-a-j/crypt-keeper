#pragma once

#include "cli/types.hpp"

namespace ck::cmd {
  void config(const ck::cli::ConfigArgs&);
  void init(const ck::cli::InitArgs&);
  void insert(const ck::cli::InsertArgs&);
  void show(const ck::cli::ShowArgs&);
}