#include <iostream>

#include "cmd/get.hpp"

namespace ck::cmd::get {
  void run_get() {
    std::cout << "Getting a secret\n";
  }
}