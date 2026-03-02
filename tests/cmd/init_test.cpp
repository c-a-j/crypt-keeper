#include <gtest/gtest.h>

#include "cmd/init.hpp"
#include "../utils/gen_key.hpp"
#include "lib/crypto.hpp"

using namespace ck::cmd::init;
using namespace ck::lib::crypto;
using namespace ck::tests::utils;

class InitTest : public ::testing::Test {
  protected: 
    std::unique_ptr<ScopedGnupgHome> gnupg_home_;
    std::string generated_fpr_;
    
    void SetUp() override {
      gnupg_home_ = std::make_unique<ScopedGnupgHome>();
      init_gpgme();
      generated_fpr_ = generate_tmp_key();
    }
    void TearDown() override {
      gnupg_home_.reset();
    } 
};

TEST_F(InitTest, InitNewCryptExists) {
  EXPECT_NO_THROW(init_crypt("test-crypt", generated_fpr_));
}

// TEST_F(CryptoTest, KeyExistsTrueWhenKeyExists) {
//   EXPECT_TRUE(key_exists(generated_fpr_, true));
// }
