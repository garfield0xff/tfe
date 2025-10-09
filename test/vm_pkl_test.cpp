#include "vm_pkl_test.h"

#include <iostream>
#include "parser/parser_torch.h"
#include "vm/op_pkl.h"

void VmPickleTest::SetUp() { stack_.clear(); }

void VmPickleTest::TearDown() { stack_.clear(); }

TEST_F(VmPickleTest, ParseOpCodesTest) {
  const std::string test_file = "/Users/gyujinkim/Desktop/Github/tfe/model/mono_640x192_decoder.pt";

  auto parser = std::make_unique<tfe::parser::TorchParser>();
  ASSERT_NO_THROW({ parser->read(test_file); });

  std::string data_pkl = parser->getData();
  ASSERT_FALSE(data_pkl.empty());

  std::vector<char> data(data_pkl.begin(), data_pkl.end());

  size_t pos = 0;
  while (pos < data.size()) {
    uint8_t opcode_byte = static_cast<uint8_t>(data[pos++]);
    tfe::vm::OpCode opcode = static_cast<tfe::vm::OpCode>(opcode_byte);

    std::string opcode_str = tfe::vm::opCodeToString(opcode);
    stack_.push_back(opcode_str);

    if (opcode == tfe::vm::OpCode::STOP) {
      break;
    }
  }

  std::cout << "\n=== Pickle OpCodes ===" << std::endl;
  for (const auto& opcode_name : stack_) {
    std::cout << opcode_name << std::endl;
  }
  std::cout << "=====================\n" << std::endl;

  // 최소한 STOP opcode는 있어야 함
  EXPECT_FALSE(stack_.empty());
  EXPECT_EQ(stack_.back(), "STOP");
}