cmake_minimum_required(VERSION 3.16)
project(crypt-keeper VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(CK_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
option(CK_ENABLE_SANITIZERS "Enable ASan/UBSan in Debug (GCC/Clang)" ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GPGME REQUIRED gpgme)

add_executable(ck
    src/main.cpp
    src/cli/build_cli.cpp
    src/utils/logger.cpp
    src/commands/init.cpp
    src/commands/get.cpp
    src/commands/config.cpp
)

target_compile_features(ck PRIVATE cxx_std_20)

include(FetchContent)
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.0
)
FetchContent_MakeAvailable(CLI11)

target_link_libraries(
    ck PRIVATE 
    CLI11::CLI11
    ${GPGME_LIBRARIES}
)


target_include_directories(
    ck PRIVATE 
    include 
    ${GPGME_INCLUDE_DIRS}
)

# Warnings
if (MSVC)
  target_compile_options(ck PRIVATE /W4 /permissive-)

  if (CK_WARNINGS_AS_ERRORS)
    target_compile_options(ck PRIVATE /WX)
  endif()
else()
  target_compile_options(ck PRIVATE
    -Wall -Wextra -Wpedantic
    -Wformat=2 -Wconversion -Wsign-conversion
    -Wshadow -Wnull-dereference -Wdouble-promotion
  )
  
  if (CK_WARNINGS_AS_ERRORS)
    target_compile_options(ck PRIVATE 
      -Werror
      -Wno-error=unused-parameter
    )
  endif()

  # Sanitizers in Debug for GCC/Clang
  if (CK_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(ck PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(ck PRIVATE -fsanitize=address,undefined)
  endif()
endif()

# Optional clang-tidy
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if (CLANG_TIDY_EXE)
  set_target_properties(ck PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()