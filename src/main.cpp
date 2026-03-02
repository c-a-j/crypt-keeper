#include <CLI/CLI.hpp>
#include <string>
#include "cli/build_cli.hpp"

#if defined(__has_feature)
#  if __has_feature(address_sanitizer)
extern "C" const char* __asan_default_options() {
  return "abort_on_error=1:print_legend=0:print_full_thread_history=0:print_summary=0:verbosity=0:symbolize=0:symbolize_inline_frames=0";
}
#  endif
#elif defined(__SANITIZE_ADDRESS__)
extern "C" const char* __asan_default_options() {
  return "abort_on_error=1:print_legend=0:print_full_thread_history=0:print_summary=0:verbosity=0:symbolize=0:symbolize_inline_frames=0";
}
#endif

int main(int argc, char** argv) {
  ck::cli::Crypt crypt = {};

  CLI::App app{"crypt-keeper"};
  ck::cli::build_cli(app, crypt);
  CLI11_PARSE(app, argc, argv)
  return 0;
}
