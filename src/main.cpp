#include "parser/parser_torch.h"
#include "error/error.h"
#include <iostream>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <model.pt>" << std::endl;
    return 1;
  }

  try {
    tfe::parser::TorchParser parser;
    parser.parseFile(argv[1]);

    std::cout << "Version: " << parser.getVersion() << std::endl;
    std::cout << "Byte Order: " << parser.getByteOrder() << std::endl;
    std::cout << "File Size:" << parser.getFileSize() << std::endl;
    std::cout << "Buffer is : " << parser.getData() << std::endl;

  } catch (const tfe::error::ParserException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
