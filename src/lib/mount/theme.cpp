#include "util/term.hpp"

#include "./_internal/theme.hpp"

namespace ck::mount {
  using namespace ck::util::term;
  using enum ck::util::term::Color;
  using enum ck::util::term::Style;
  using enum OutputComponent;
  
  TextStyle get_scheme_style(OutputComponent c) {
    switch (c) {
      case RootMountPath:   return { Blue, { Bold } };
      case RootMountAlias:  return { Blue, { Bold } };
      case RootMountArrow:  return { Blue, { Bold } };
      case MountPath:       return { Blue, { Bold } };
      case MountAlias:      return { Blue, { Bold } };
      case MountArrow:      return { Blue, { Bold } };
      case Line:            return { Gray, { Faint } };
    }
    return { Gray };
  }
  
  std::string get_scheme_ansi(OutputComponent c) {
    return ansi(get_scheme_style(c));
  }
}
