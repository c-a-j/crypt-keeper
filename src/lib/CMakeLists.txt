target_sources(ck_lib PRIVATE 
  crypto.cpp
  index.cpp
  config.cpp
)

file(READ "${CMAKE_SOURCE_DIR}/res/default_config.toml" DEFAULT_CONFIG_CONTENT)
configure_file(./default_config.hpp.in "${CMAKE_SOURCE_DIR}/include/lib/default_config.hpp" @ONLY)
