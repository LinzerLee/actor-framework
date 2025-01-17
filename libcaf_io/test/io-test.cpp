#define CAF_TEST_NO_MAIN

#include "io-test.hpp"

#include "caf/test/unit_test_impl.hpp"

int main(int argc, char** argv) {
  using namespace caf;
  init_global_meta_objects<id_block::io_test>();
  io::middleman::init_global_meta_objects();
  core::init_global_meta_objects();
  return test::main(argc, argv);
}
