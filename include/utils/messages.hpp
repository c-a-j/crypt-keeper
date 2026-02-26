#pragma once
#include <string>

namespace colors {
  constexpr const char* reset = "\033[0m";
  // Gruvbox Dark palette (foreground accents)
  constexpr const char* fg      = "\033[38;2;235;219;178m"; // fg (#ebdbb2)
  constexpr const char* gray    = "\033[38;2;146;131;116m"; // gray (#928374)

  constexpr const char* red     = "\033[38;2;251;73;52m";   // bright_red (#fb4934)
  constexpr const char* green   = "\033[38;2;184;187;38m";  // bright_green (#b8bb26)
  constexpr const char* yellow  = "\033[38;2;250;189;47m";  // bright_yellow (#fabd2f)
  constexpr const char* blue    = "\033[38;2;131;165;152m"; // bright_blue (#83a598)
  constexpr const char* aqua    = "\033[38;2;142;192;124m"; // bright_aqua (#8ec07c)
  constexpr const char* orange  = "\033[38;2;254;128;25m";  // bright_orange (#fe8019)
  constexpr const char* purple = "\033[38;2;211;134;155m"; // bright_purple
  constexpr const char* purple_dim = "\033[38;2;177;98;134m";


  constexpr const char* success = green;
  constexpr const char* error = red;
  constexpr const char* info = gray;
  constexpr const char* warning = yellow;
  constexpr const char* debug = gray;
}

enum class MsgLevel {
  Info,
  Success,
  Warning,
  Error,
  Debug
};

std::string err_str(const std::string&);

void info(std::string_view);
void warn(std::string_view);
void error(std::string_view);
void success(std::string_view);
void debug(std::string_view);

void emit(MsgLevel level, std::string_view msg);
void emit_inline(MsgLevel level, std::string_view msg);
void finish_line();