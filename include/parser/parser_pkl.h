#ifndef TFE_PARSER_PKL_H_
#define TFE_PARSER_PKL_H_

#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace tfe {
namespace parser {

/**
 * @brief Python Pickle Protocol 2 파서
 * @note PyTorch data.pkl 파싱을 위한 기본 pickle 연산 지원
 */
class PickleParser {
 public:
  // Pickle value types
  using PickleValue =
      std::variant<std::nullptr_t, bool, int64_t, double, std::string,
                   std::vector<char>,                                             // bytes
                   std::shared_ptr<std::vector<std::shared_ptr<void>>>,           // list
                   std::shared_ptr<std::map<std::string, std::shared_ptr<void>>>  // dict
                   >;

  PickleParser();
  ~PickleParser();

  /**
   * @brief 버퍼로부터 pickle 데이터 파싱
   * @param data Raw pickle 바이트
   * @return 파싱된 pickle 객체
   */
  PickleValue parse(const std::vector<char>& data);

  /**
   * @brief 문자열로부터 pickle 데이터 파싱
   * @param data Raw pickle 문자열
   * @return 파싱된 pickle 객체
   */
  PickleValue parse(const std::string& data);

  /**
   * @brief pickle 구조의 가독성 있는 표현 반환
   * @return 문자열 표현
   */
  std::string getStructure() const;

 private:
  // Pickle opcodes (Protocol 2)
  // https://github.com/python/cpython/blob/main/Lib/pickle.py 참고
  enum class OpCode : uint8_t {
    PROTO            = 0x80,  // Protocol version
    GLOBAL           = 'c',   // Module and name
    PUT              = 'p',   // Store to memo
    BINPUT           = 'q',   // Store to memo (1-byte index)
    LONG_BINPUT      = 'r',   // Store to memo (4-byte index)
    GET              = 'g',   // Retrieve from memo
    BINGET           = 'h',   // Retrieve from memo (1-byte index)
    LONG_BINGET      = 'j',   // Retrieve from memo (4-byte index)
    NONE             = 'N',   // None
    NEWTRUE          = 0x88,  // True
    NEWFALSE         = 0x89,  // False
    BININT           = 'J',   // 4-byte signed int
    BININT1          = 'K',   // 1-byte unsigned int
    BININT2          = 'M',   // 2-byte unsigned int
    LONG1            = 0x8a,  // Long integer (1-byte length)
    BINFLOAT         = 'G',   // 8-byte float
    SHORT_BINSTRING  = 'U',   // String (1-byte length)
    BINSTRING        = 'T',   // String (4-byte length)
    SHORT_BINUNICODE = 0x8c,  // Unicode (1-byte length)
    BINUNICODE       = 'X',   // Unicode (4-byte length)
    EMPTY_LIST       = ']',   // Empty list
    APPEND           = 'a',   // Append to list
    APPENDS          = 'e',   // Extend list
    EMPTY_DICT       = '}',   // Empty dict
    SETITEM          = 's',   // Add key-value to dict
    SETITEMS         = 'u',   // Add multiple key-values to dict
    EMPTY_TUPLE      = ')',   // Empty tuple
    TUPLE1           = 0x85,  // 1-element tuple
    TUPLE2           = 0x86,  // 2-element tuple
    TUPLE3           = 0x87,  // 3-element tuple
    TUPLE            = 't',   // Tuple from stack
    MARK             = '(',   // Push mark to stack
    BUILD            = 'b',   // Build object
    REDUCE           = 'R',   // Apply callable to args
    BINPERSID        = 'Q',   // Persistent object ID
    STOP             = '.',   // End of pickle
  };

  struct PickleObject {
    std::string module;
    std::string name;
    std::vector<std::shared_ptr<void>> args;
  };

  std::vector<std::shared_ptr<void>> stack_;
  std::map<int, std::shared_ptr<void>> memo_;
  std::string structure_;
  size_t pos_;

  void parseOpcodes(const std::vector<char>& data);
  int32_t readInt32(const std::vector<char>& data);
  int16_t readInt16(const std::vector<char>& data);
  double readFloat64(const std::vector<char>& data);
  std::string readString(const std::vector<char>& data, size_t length);

  void handleGlobal(const std::vector<char>& data);
  void handleBinPut(const std::vector<char>& data);
  void handleBinGet(const std::vector<char>& data);
  void handleBinInt(const std::vector<char>& data);
  void handleBinString(const std::vector<char>& data);
  void handleBinUnicode(const std::vector<char>& data);
  void handleReduce();
  void handleTuple(const std::vector<char>& data);
  void handleEmptyDict();
  void handleSetItems();
};

}  // namespace parser
}  // namespace tfe

#endif  // TFE_PARSER_PKL_H_
