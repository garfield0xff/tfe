#include "parse_torchscript_test.h"

#include <iostream>

#include "error/error.h"

void TorchParserTest::SetUp() { parser = std::make_unique<tfe::parser::TorchParser>(); }

void TorchParserTest::TearDown() { parser.reset(); }

TEST_F(TorchParserTest, ParseTest) {
  const std::string test_file = "/Users/gyujinkim/Desktop/Github/tfe/model/mono_640x192_decoder.pt";

  ASSERT_NO_THROW({ parser->read(test_file); });

  std::string version    = parser->getVersion();
  std::string byte_order = parser->getByteOrder();

  EXPECT_FALSE(version.empty());
  EXPECT_FALSE(byte_order.empty());

  std::cout << "version   : " << version << std::endl;
  std::cout << "byteOrder : " << byte_order << std::endl;
}