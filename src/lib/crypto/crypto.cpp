#include "lib/crypto/crypto.hpp"
#include <gpgme.h>
#include <stdexcept>

#include "util/error.hpp"

namespace {
  using ck::util::error::Error;
  using ck::util::error::CryptoErrc;
  using enum ck::util::error::CryptoErrc;
  
  bool key_exists(const std::string& key_id, const bool secret) {
    ck::crypto::init_gpgme();

    gpgme_ctx_t ctx = nullptr;
    gpgme_key_t key = nullptr;

    gpgme_error_t err = gpgme_new(&ctx);
    if (err) {
      throw Error<CryptoErrc>{GpgmeNewFailed, gpgme_strerror(err)};
    }

    err = gpgme_get_key(ctx, key_id.c_str(), &key, secret ? 1 : 0);

    if (key) {
      gpgme_key_unref(key);
    }
    gpgme_release(ctx);

    // not found is a normal false condition
    if (gpgme_err_code(err) == GPG_ERR_EOF || gpgme_err_code(err) == GPG_ERR_NOT_FOUND) {
      return false;
    }

    // any other error should surface
    if (err) {
      throw Error<CryptoErrc>{GpgmeGetKeyFailed, gpgme_strerror(err)};
    }

    return true;
  }
} 
  
namespace ck::crypto {
  void init_gpgme() {
    if (!gpgme_check_version(nullptr)) {
      throw Error<CryptoErrc>{GpgmeFailed, "gpgme initialization failed"};
    }
  }
  bool public_key_exists(const std::string& key_fpr) {return key_exists(key_fpr, false);}
  bool private_key_exists(const std::string& key_fpr) {return key_exists(key_fpr, true);}
}
