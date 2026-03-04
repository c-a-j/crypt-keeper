add_executable(crypto_test
  crypto_test.cpp
  ../util/gen_key.cpp
)

target_link_libraries(crypto_test PRIVATE
  ck_lib
  GTest::gtest_main
)

target_include_directories(crypto_test PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)

gtest_discover_tests(crypto_test 
  EXTRA_ARGS --gtest_print_time=0 --gtest_brief=1
)
