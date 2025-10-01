#ifndef TFE_PARSER_BASE_H_
#define TFE_PARSER_BASE_H_

#include <string>

namespace tfe {
namespace parser {

class BaseParser {
 public:
 /**
  * @brief Parser 기저 클래스 
  * 
  * TorchScript, Tensorflow lite 의 파일을 파싱하여 내부 Tensor로 전환할 수 있도록 하는 클래스
  * 
  */
  BaseParser()          = default;
  virtual ~BaseParser() = default;

  virtual void parseFile(const std::string& file_name) = 0;
  virtual std::string getVersion() const               = 0;
  virtual std::string getByteOrder() const             = 0;
  virtual std::string getFileSize() const              = 0;

 protected:
  std::string version_;
  std::string byte_order_;
  std::string file_size_;
};

}  // namespace parser
}  // namespace tfe

#endif  // TFE_PARSER_BASE_H_
