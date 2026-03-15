
#include "lib/index/theme.hpp"
#include "util/term.hpp"

namespace ck::index {
  using namespace ck::util::term;
  using enum ck::util::term::Color;
  using enum ck::util::term::Style;
  using enum OutputComponent;
  
  std::string get_scheme_ansi(OutputComponent c) {
    switch (c) {
      case VaultName:      
        return ansi(Blue, { Bold });
      case NodeName:        
        return ansi(Blue, { Bold });
      case EntryName:       
        return ansi(Yellow, { Faint });
      case Line:            
        return ansi(Gray, { Faint });
    }
    
    return ansi(Gray);
  }
}