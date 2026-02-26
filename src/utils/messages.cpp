#include <string>
#include <iostream>
#include <sstream>
#include "utils/messages.hpp"

void emit_impl(MsgLevel level, std::string_view msg, bool add_newline) {
  const char* color = colors::info;
  const char* tag = "[*]";
  std::ostream* out = &std::cout;
  
  switch (level) {
    case MsgLevel::Info: color = colors::info; tag = "[.]"; break;
    case MsgLevel::Success: color = colors::success; tag = "[*]"; break;
    case MsgLevel::Warning: color = colors::warning; tag = "[W]"; break;
    case MsgLevel::Error: 
      color = colors::error; tag = "[E]"; 
      out = &std::cerr;
      break;
    case MsgLevel::Debug: color = colors::debug; tag = "[D]"; break;
  }
  
  (*out) << colors::purple << "ck  " << colors::reset;
  (*out) << color << tag << "  " << msg << colors::reset;
  if (add_newline) (*out) << "\n";
}

void emit(MsgLevel level, std::string_view msg) {
  emit_impl(level, msg, true);
}

void emit_inline(MsgLevel level, std::string_view msg) {
  emit_impl(level, msg, false);
}

void info(std::string_view msg)    { emit(MsgLevel::Info, msg); }
void warning(std::string_view msg)    { emit(MsgLevel::Warning, msg); }
void error(std::string_view msg)   { emit(MsgLevel::Error, msg); }
void success(std::string_view msg) { emit(MsgLevel::Success, msg); }
void debug(std::string_view msg)   { emit(MsgLevel::Debug, msg); }


std::string err_str(const std::string& msg) {
  std::ostringstream ss;
  ss << colors::red << msg << colors::reset;
  return ss.str();
}