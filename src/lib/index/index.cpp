#include <nlohmann/json.hpp>
#include <string>
#include <iostream>

#include "util/logger.hpp"
#include "lib/types.hpp"
#include "lib/index/index.hpp"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#endif

namespace ck::lib::index { 
  using namespace ck::types;
  using namespace ck::util::logger;
  
  
  void wisper(Secret& secret) {
    
    logger.info("");
    
    #ifdef _WIN32
      for (;;) {
        int ch = _getch();
        
        if (ch == '\r') break; // Enter
        if (ch == '\b' && !value.empty()) {
          value.pop_back();
          continue;
        }
        if (ch >= 32 && ch <= 126) {
          value.push_back(static_cast<char>(ch));
        }
      }
    #else
      termios oldt {};
      tcgetattr(STDIN_FILENO, &oldt);
      
      termios newt = oldt;
      newt.c_lflag &= ~static_cast<tcflag_t>(ECHO);
      tcsetattr(STDIN_FILENO, TCSANOW, &newt);
      
      std::string value;
      std::getline(std::cin, value);
      
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    #endif
    
    secret.value = value;
  };
  
  void insert(const VaultConfig& cfg, const Secret& secret) {
  };
}
