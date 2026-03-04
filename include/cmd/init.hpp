#pragma once
#include <string>
#include <expected>


namespace ck::cmd::init {
  
  enum class InitErrc {
    KeyNotFound,
    CreateDirectoryFailed,
    OpenGpgIdFailed,
    WriteGpgIdFailed,
    AlreadyExists,
    IoError
  };
  
  struct InitError {
    InitErrc code;
    std::string msg1;
    std::string msg2 = {};
    
    bool has_msg2() const { return !msg2.empty(); }
  };
  
  std::expected<void, InitError> create_store(std::string, std::string);
  int init_store(std::string, std::string);
}