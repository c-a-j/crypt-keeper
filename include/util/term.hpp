#pragma once

#include <string>
#include <vector>

namespace ck::util::term {
  enum class Color {
    Default,
    Gray,
    Red,
    Green,
    Yellow,
    Blue,
    Aqua,
    Orange,
    Purple,
    Reset
  };
  
  enum class Style {
    Normal,
    Bold,
    Faint,
    Italic,
    Underline
  };

  inline const char* color_code(Color c) {
    switch (c) {
      case Color::Gray:       return "38;2;146;131;116";
      case Color::Red:        return "38;2;251;73;52";
      case Color::Green:      return "38;2;184;187;38";
      case Color::Yellow:     return "38;2;250;189;47";
      case Color::Blue:       return "34";
      case Color::Aqua:       return "38;2;142;192;124";
      case Color::Orange:     return "38;2;254;128;25";
      case Color::Purple:     return "38;2;211;134;155";
      case Color::Reset:      return "0";
      case Color::Default:    return "";
    }
    return "";
  }
  
  inline const char* style_code(Style s) {
    switch (s) {
      case Style::Normal:     return "0";
      case Style::Bold:       return "1;";
      case Style::Faint:      return "2;";
      case Style::Italic:     return "3;";
      case Style::Underline:  return "4;";
    }
    return "";
  }

  inline std::string ansi(Color color, std::vector<Style> style = {}) {
    if (color == Color::Default) { return ""; }
    std::string out = "\033[";
    
    for (auto s : style) {
      out += style_code(s);
    }
    
    out += color_code(color);
    out += "m";
    return out;
  }
}