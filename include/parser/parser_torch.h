#ifndef TFE_PARSER_TORCH_H_
#define TFE_PARSER_TORCH_H_

#include <unzip.h>

#include <string>
#include <vector>
#include <map>

#include "parser/parser_base.h"

namespace tfe {
namespace parser {
/**
 * @brief TorchScript 디코딩 파서 클래스
 *
 */
class TorchParser : public BaseParser {
 public:
  TorchParser();
  ~TorchParser() override;

  void parseFile(const std::string& file_name) override;
  void read(const std::string& file_name);

  std::string getVersion() const override;
  std::string getByteOrder() const override;
  std::string getFileSize() const override;
  std::string getData() const override;

 private:
  std::string read_file_from_zip(unzFile uf, const std::string& internal_path);
  std::string model_name_;
};

}  // namespace parser
}  // namespace tfe

#endif  // TFE_PARSER_TORCH_H_
