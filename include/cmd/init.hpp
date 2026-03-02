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
    std::string detail;
  };
  
  void run_init(std::string, std::string);
  std::expected<void, InitError> init_crypt(std::string, std::string);
  
}