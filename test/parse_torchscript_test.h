#ifndef PARSE_TORCHSCRIPT_TEST_H_
#define PARSE_TORCHSCRIPT_TEST_H_

#include <gtest/gtest.h>
#include <memory>
#include "parser/parser_torch.h"

class TorchParserTest : public ::testing::Test {
 protected:
  std::unique_ptr<tfe::parser::TorchParser> parser;

  void SetUp() override;
  void TearDown() override;
};

#endif  // PARSE_TORCHSCRIPT_TEST_H_
