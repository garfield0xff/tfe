#ifndef VM_PKL_TEST_H_
#define VM_PKL_TEST_H_

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include "vm/vm_pkl.h"

class VmPickleTest : public ::testing::Test {
 protected:
  std::vector<std::string> stack_;

  void SetUp() override;
  void TearDown() override;
};

#endif  // VM_PKL_TEST_H_
