#include <string>
#include <filesystem>
#include <limits>
#include <vector>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <unistd.h>
  #include <cerrno>
  #include <cstring>
#endif

#include "util/error.hpp"

namespace ck::path {
  namespace fs = std::filesystem;
  using ck::util::error::Error;
  using ck::util::error::PathErrc;
  using enum ck::util::error::PathErrc;
  
#ifndef _WIN32
  int write_all(int fd, const char* data, std::size_t len) {
    std::size_t written = 0;
    while (written < len) {
      ssize_t n = ::write(fd, data + written, len - written);
      if (n > 0) {
        written += static_cast<std::size_t>(n);
        continue;
      }
      if (n == -1) {
        if (errno == EINTR) { continue;}
        return errno;
      }
      return EIO;
    }
    return 0;
  }
#else
  int last_error() noexcept {
    return static_cast<int>(::GetLastError());
  }
#endif
  
  std::string msg(const fs::path& path, int err)  {
    return "[" + std::to_string(err) + "] " + path.string();
  }
  
#ifndef _WIN32
  struct TempFile {
    fs::path path;
    fs::path target;
    bool active = true;
    int fd = -1;
    
    explicit TempFile(const fs::path& target_) : target(target_) {
      std::string tmpl = target.string() + ".tmp.XXXXXX";
      std::vector<char> buf(tmpl.begin(), tmpl.end());
      buf.push_back('\0');
      fd = ::mkstemp(buf.data());
      if (fd == -1) {
        throw Error<PathErrc>{MkstempFailed, msg(target, errno)};
      }
      
      try {
        path = buf.data();
      } catch (const std::exception& e) {
        ::close(fd);
        ::unlink(buf.data());
        fd = -1;
        throw Error<PathErrc>{ FileSystemError, e.what() };
      }
    };

    TempFile(const TempFile&) = delete;
    TempFile& operator=(const TempFile&) = delete;
    
    int write(const std::string& contents) noexcept {
      return write_all(fd, contents.data(), contents.size());
    };

    int sync() noexcept {
      if (::fsync(fd) == -1) {
        return errno;
      }
      return 0;
    }

    int close() noexcept { 
      if (fd == -1) { return 0; }
      int rc = ::close(fd);
      int err = (rc == -1) ? errno : 0;
      fd = -1;
      return err;
    }
    
    int commit() noexcept {
      if (::rename(path.c_str(), target.c_str()) == -1) {
        return errno;
      }
      active = false;
      return 0;
    };

    ~TempFile() noexcept {
      close();
      if (active) {
        ::unlink(path.c_str());
      }
    };
  };
#else
  struct TempFile {
    fs::path path;
    fs::path target;
    bool active = true;
    HANDLE handle = INVALID_HANDLE_VALUE;

    explicit TempFile(const fs::path& target_) : target(target_) {
      fs::path parent = target.parent_path();
      fs::path dir = parent.empty() ? fs::path(".") : parent;

      std::vector<wchar_t> buf(MAX_PATH);
      DWORD result = ::GetTempFileNameW(dir.c_str(), L"ckp", 0, buf.data());
      if (result == 0) {
        throw Error<PathErrc>{MkstempFailed, msg(target, last_error())};
      }

      try {
        path = buf.data();
      } catch (const std::exception& e) {
        ::DeleteFileW(buf.data());
        throw Error<PathErrc>{FileSystemError, e.what()};
      }

      handle = ::CreateFileW(
        path.c_str(),
        GENERIC_WRITE,
        0,
        nullptr,
        TRUNCATE_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
      );
      if (handle == INVALID_HANDLE_VALUE) {
        int err = last_error();
        ::DeleteFileW(path.c_str());
        throw Error<PathErrc>{OpenFailed, msg(path, err)};
      }
    }

    TempFile(const TempFile&) = delete;
    TempFile& operator=(const TempFile&) = delete;

    int write(const std::string& contents) noexcept {
      const char* data = contents.data();
      std::size_t remaining = contents.size();

      while (remaining > 0) {
        DWORD chunk = remaining > static_cast<std::size_t>(std::numeric_limits<DWORD>::max())
          ? std::numeric_limits<DWORD>::max()
          : static_cast<DWORD>(remaining);
        DWORD written = 0;
        if (!::WriteFile(handle, data, chunk, &written, nullptr)) {
          return last_error();
        }
        if (written == 0) {
          return ERROR_WRITE_FAULT;
        }
        data += written;
        remaining -= written;
      }

      return 0;
    }

    int sync() noexcept {
      if (!::FlushFileBuffers(handle)) {
        return last_error();
      }
      return 0;
    }

    int close() noexcept {
      if (handle == INVALID_HANDLE_VALUE) {
        return 0;
      }

      if (!::CloseHandle(handle)) {
        int err = last_error();
        handle = INVALID_HANDLE_VALUE;
        return err;
      }

      handle = INVALID_HANDLE_VALUE;
      return 0;
    }

    int commit() noexcept {
      if (!::MoveFileExW(
        path.c_str(),
        target.c_str(),
        MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH
      )) {
        return last_error();
      }
      active = false;
      return 0;
    }

    ~TempFile() noexcept {
      close();
      if (active) {
        ::DeleteFileW(path.c_str());
      }
    }
  };
#endif
  

  void write_atomic(const fs::path& target, const std::string& contents) {
    std::error_code ec;
    fs::path parent = target.parent_path();
    if (!parent.empty()) {
      fs::create_directories(parent, ec);
    }
    if (ec) {
      throw Error<PathErrc>{CreateDirectoryFailed, parent.string() + ": " + ec.message()};
    }

    TempFile tmp(target);
    int err = tmp.write(contents);
    if (err) {
      throw Error<PathErrc>{WriteFailed, msg(tmp.path, err)};
    }

    err = tmp.sync();
    if (err) {
      throw Error<PathErrc>{FsyncFailed, msg(tmp.path, err)};
    }

    err = tmp.close();
    if (err) {
      throw Error<PathErrc>{CloseFailed, msg(tmp.path, err)};
    }

    err = tmp.commit();
    if (err) {
      throw Error<PathErrc>{RenameFailed, msg(tmp.path, err) + " -> " + target.string()};
    }
  }
}
