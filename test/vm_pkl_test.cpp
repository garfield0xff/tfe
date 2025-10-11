#include "vm_pkl_test.h"

#include <iostream>
#include <fstream>
#include "parser/parser_torch.h"
#include "vm/op_pkl.h"
#include "vm/vm_pkl.h"

void VmPickleTest::SetUp() { stack_.clear(); }

void VmPickleTest::TearDown() { stack_.clear(); }

TEST_F(VmPickleTest, ParseOpCodesTest) {
  const std::string test_file = "/Users/gyujinkim/Desktop/Github/tfe/model/mono_640x192_encoder.pt";
  const std::string output_file = "../test/parse_pkl.txt";

  auto parser = std::make_unique<tfe::parser::TorchParser>();
  ASSERT_NO_THROW({ parser->read(test_file); });

  std::string data_pkl = parser->getData();
  ASSERT_FALSE(data_pkl.empty());

  std::vector<char> data(data_pkl.begin(), data_pkl.end());

  tfe::vm::PickleVM pkl_vm(data);
  std::vector<std::string> opcodes;

  ASSERT_NO_THROW({
    opcodes = pkl_vm.parse();
  });

  std::ofstream out_file(output_file);
  ASSERT_TRUE(out_file.is_open()) << "Failed to open output file: " << output_file;

  size_t count = 0;
  for (const auto& opcode_str : opcodes) {
    out_file << opcode_str << std::endl;
  }

  out_file.close();
  std::cout << "\nParsed opcodes saved to: " << output_file << std::endl;

  EXPECT_FALSE(opcodes.empty());
  EXPECT_EQ(opcodes.back(), "STOP");
}