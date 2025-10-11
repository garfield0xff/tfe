/**
 * @brief Pickle virtual machine for parsing torch script
 */

#ifndef TFE_VM_VM_PKL_H
#define TFE_VM_VM_PKL_H

#include "vm/op_pkl.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

namespace tfe {
namespace vm {

class PickleVM {
public:
    PickleVM(const std::vector<char>& data) : data_(data), pos_(0) {}

    std::vector<std::string> parse();

private:
    const std::vector<char>& data_;
    size_t pos_;

    // read byte per type (cstdint)
    uint8_t readByte();
    uint16_t readUint16();
    uint32_t readUint32();
    int32_t readInt32();
    std::string readLine();
    std::string readBytes(size_t count);

    // parse logic by opcode
    std::string parseProto();
    std::string parseGlobal();
    std::string parseBinunicode();
    std::string parseBinstring();
    std::string parseShortBinstring();
    std::string parseBinint();
    std::string parseBinint1();
    std::string parseBinint2();
    std::string parseBinget();
    std::string parseLongBinget();
    std::string parseBinput();
    std::string parseLongBinput();
    std::string parseBinpersid();
    std::string parseGet();
    std::string parsePut();
    std::string parseBinbytes();
    std::string parseShortBinbytes();
    std::string parseShortBinunicode();
    std::string parseBinunicode8();
    std::string parseBinbytes8();
    std::string parseLong1();
    std::string parseLong4();
    std::string parseFrame();
    std::string parseMemoize();

    // Utility
    std::string bytesToHex(const std::string& bytes);
    bool hasMore() const { return pos_ < data_.size(); }
};

} // namespace vm
} // namespace tfe

#endif // TFE_VM_VM_PKL_H