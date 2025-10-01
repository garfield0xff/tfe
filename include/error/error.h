#ifndef TFE_ERROR_ERROR_H_
#define TFE_ERROR_ERROR_H_

#include <exception>
#include <string>

namespace tfe {
namespace error {

typedef enum {
  PARSER_SUCCESS = 0,
  OPEN_FAILED,
  READ_FAILED,
  ZIP_ERROR = -101,
} ParseError;

class ParserException : public std::exception {
 public:
  ParserException(ParseError error_code, const std::string& message)
      : error_code_(error_code), message_(message) {}
  virtual ~ParserException() noexcept = default;

  virtual const char* what() const noexcept override { return message_.c_str(); }

  const std::string& message() const { return message_; }
  ParseError error_code() const { return error_code_; }

 private:
  ParseError error_code_;
  std::string message_;
};

}  // namespace error
}  // namespace tfe

#endif  // TFE_ERROR_ERROR_H_
