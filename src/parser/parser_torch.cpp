#include "parser/parser_torch.h"

#include <sys/stat.h>

#include <cstring>
#include <iostream>

#include "error/error.h"

namespace tfe {
namespace parser {

TorchParser::TorchParser() {}

TorchParser::~TorchParser() {}

/**
 * @brief TorchScript 파일 파싱 (1안)
 * @note TorchScript ( version, byteorder 완료 ) , ( storage_type, tensor_shape, tensor_data,
 * storage_offset, state_dict_keys, optimizer_state 구현 필요) 
 * TODO: 구조 잘못됌 iterator로 파일 내부적으로 순회해서 처리
 * @date 2025-10-01
 * @param file_name
 */
void TorchParser::parseFile(const std::string& file_name) {
  struct stat st;
  if (stat(file_name.c_str(), &st) == 0) {
    file_size_ = std::to_string(st.st_size);
  }

  unzFile uf = unzOpen(file_name.c_str());
  if (!uf) {
    throw error::ParserException(error::OPEN_FAILED, "Failed to open file: " + file_name);
  }

  size_t last_slash = file_name.find_last_of("/\\");
  size_t last_dot   = file_name.find_last_of(".");
  model_name_       = file_name.substr(last_slash + 1, last_dot - last_slash - 1);

  try {
    // IR Version
    version_    = read_file_from_zip(uf, model_name_ + "/version");
    // Little / Big endian
    byte_order_ = read_file_from_zip(uf, model_name_ + "/byteorder");
    // tensor, stoarge, dtype, shape
    // TODO: Encoding 깨짐 
    data_       = read_file_from_zip(uf, model_name_ + "/data.pkl");
    // constants value for model
    constants_  = read_file_from_zip(uf, model_name_ + "/constants.pkl");
    // torch script code ( model structure )
    code_        = read_file_from_zip(uf, model_name_ + "/code/__torch__.py");
    // operation code & graph -> 이거 없음 확인
    // bytecode_    = read_file_from_zip(uf, model_name_ + "/bytecode.pkl");
    // TODO: archive ( data directory for tensor layer ) ex : /archive/data_0
  } catch (const error::ParserException& e) {
    unzClose(uf);
    throw;
  }

  unzClose(uf);
}

/**
 * @brief TorchScript 파싱 2안 (iterator)
 * @note 텐서를 구축후에 넣어야 할 것 같은데 사이즈 할당에 대해서 공부 필요
 * https://github.com/apache/tvm/blob/main/include/tvm/te/tensor.h 참고
 * @date 2025-10-01
 */
void TorchParser::read(const std::string& file_name) {
  std::map<std::string, std::vector<char>> file_data;
  unzFile zipfile = unzOpen(file_name.c_str());
  if (!zipfile) {
    throw error::ParserException(error::OPEN_FAILED, "Failed to open file: " + file_name);
  }
  
  unz_global_info global_info;
  unzGetGlobalInfo(zipfile, &global_info);
  // global_info.number_entry return type is unsigned long
  for(unsigned long i = 0; i < global_info.number_entry; i++) {
    char file_inzip[256];
    unz_file_info file_info;
    unzGetCurrentFileInfo(zipfile, &file_info, file_inzip, sizeof(file_inzip), NULL, 0, NULL, 0);
    if(unzOpenCurrentFile(zipfile) != UNZ_OK) {
      throw error::ParserException(error::ZIP_ERROR, "Failed to parse internal file: " + file_name);
    }

    std::vector<char> buffer(file_info.uncompressed_size);
    unzReadCurrentFile(zipfile, buffer.data(), buffer.size());
    unzCloseCurrentFile(zipfile);

    file_data[std::string(file_inzip)] = buffer;

    if((i + 1) < global_info.number_entry) {
      unzGoToNextFile(zipfile);
    }
  }
  unzClose(zipfile);
}



std::string TorchParser::getVersion() const { return version_; }

std::string TorchParser::getByteOrder() const { return byte_order_; }

std::string TorchParser::getFileSize() const { return file_size_; }

std::string TorchParser::getData() const { return data_; }

std::string TorchParser::read_file_from_zip(unzFile uf, const std::string& internal_path) {
  if (unzLocateFile(uf, internal_path.c_str(), 0) != UNZ_OK) {
    throw error::ParserException(error::ZIP_ERROR, "File not found in ZIP: " + internal_path);
  }

  if (unzOpenCurrentFile(uf) != UNZ_OK) {
    throw error::ParserException(error::ZIP_ERROR, "Failed to open file: " + internal_path);
  }

  unz_file_info file_info;
  if (unzGetCurrentFileInfo(uf, &file_info, nullptr, 0, nullptr, 0, nullptr, 0) != UNZ_OK) {
    unzCloseCurrentFile(uf);
    throw error::ParserException(error::ZIP_ERROR, "Failed to get file info: " + internal_path);
  }

  std::vector<char> buffer(file_info.uncompressed_size);
  int bytes_read = unzReadCurrentFile(uf, buffer.data(), buffer.size());
  if (bytes_read < 0) {
    unzCloseCurrentFile(uf);
    throw error::ParserException(error::READ_FAILED, "Failed to read file: " + internal_path);
  }

  unzCloseCurrentFile(uf);

  std::string result(buffer.data(), bytes_read);

  size_t end = result.find_last_not_of("\n\r");
  if (end != std::string::npos) {
    result.erase(end + 1);
  }

  return result;
}

}  // namespace parser
}  // namespace tfe
