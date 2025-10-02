#include "parser/parser_torch.h"

#include <sys/stat.h>

#include <cstring>
#include <iostream>

#include "error/error.h"
#include "parser/parser_pkl.h"

namespace tfe {
namespace parser {

TorchParser::TorchParser() {}

TorchParser::~TorchParser() {}

/**
 * @brief TorchScript 파일  
 * @note pipeline: read() -> parse()
 * @date 2025-10-01
 * @param file_name
 */
void TorchParser::read(const std::string& file_name) {
  struct stat st;
  if (stat(file_name.c_str(), &st) == 0) {
    file_size_ = std::to_string(st.st_size);
  }

  unzFile zipfile = unzOpen(file_name.c_str());
  if (!zipfile) {
    throw error::ParserException(error::OPEN_FAILED, "Failed to open file: " + file_name);
  }

  size_t last_slash = file_name.find_last_of("/\\");
  size_t last_dot   = file_name.find_last_of(".");
  model_name_       = file_name.substr(last_slash + 1, last_dot - last_slash - 1);

  try {
    parse(zipfile);
  } catch (const error::ParserException& e) {
    unzClose(zipfile);
    throw;
  }

  unzClose(zipfile);
}

/**
 * @brief 텐서 파싱
 * @note 텐서를 구축후에 넣어야 할 것 같은데 사이즈 할당에 대해서 공부 필요
 * https://github.com/apache/tvm/blob/main/include/tvm/te/tensor.h 참고
 * @date 2025-10-02
 */
void TorchParser::parse(unzFile zipfile) {
  try {
    version_ = read_file_from_zip(zipfile, model_name_ + "/version");
    byte_order_ = read_file_from_zip(zipfile, model_name_ + "/byteorder");
    std::string data_pkl = read_file_from_zip(zipfile, model_name_ + "/data.pkl");

    // parse pkl 
    PickleParser pkl_parser;
    pkl_parser.parse(data_pkl);
    std::cout << pkl_parser.getStructure() << std::endl;

  } catch (const error::ParserException& e) {
    unzClose(zipfile);
    throw;
  }
}

std::string TorchParser::getVersion() const { return version_; }

std::string TorchParser::getByteOrder() const { return byte_order_; }

std::string TorchParser::getFileSize() const { return file_size_; }

std::string TorchParser::getData() const { return data_; }

std::string TorchParser::read_file_from_zip(unzFile zipfile, const std::string& internal_path) {
  if (unzLocateFile(zipfile, internal_path.c_str(), 0) != UNZ_OK) {
    throw error::ParserException(error::ZIP_ERROR, "File not found in ZIP: " + internal_path);
  }

  if (unzOpenCurrentFile(zipfile) != UNZ_OK) {
    throw error::ParserException(error::ZIP_ERROR, "Failed to open file: " + internal_path);
  }

  unz_file_info file_info;
  if (unzGetCurrentFileInfo(zipfile, &file_info, nullptr, 0, nullptr, 0, nullptr, 0) != UNZ_OK) {
    unzCloseCurrentFile(zipfile);
    throw error::ParserException(error::ZIP_ERROR, "Failed to get file info: " + internal_path);
  }

  std::vector<char> buffer(file_info.uncompressed_size);
  int bytes_read = unzReadCurrentFile(zipfile, buffer.data(), buffer.size());
  if (bytes_read < 0) {
    unzCloseCurrentFile(zipfile);
    throw error::ParserException(error::READ_FAILED, "Failed to read file: " + internal_path);
  }

  unzCloseCurrentFile(zipfile);

  std::string result(buffer.data(), bytes_read);

  size_t end = result.find_last_not_of("\n\r");
  if (end != std::string::npos) {
    result.erase(end + 1);
  }

  return result;
}

}  // namespace parser
}  // namespace tfe
