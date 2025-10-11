#include "vm/vm_pkl.h"
#include <stdexcept>
#include <cstring>

namespace tfe {
namespace vm {

std::vector<std::string> PickleVM::parse() {
    std::vector<std::string> opcodes;

    while (hasMore()) {
        uint8_t opcode_byte = readByte();
        OpCode opcode = static_cast<OpCode>(opcode_byte);

        std::string opcode_str;

        switch (opcode) {
            case OpCode::PROTO:
                opcode_str = parseProto();
                break;

            case OpCode::GLOBAL:
                opcode_str = parseGlobal();
                break;

            case OpCode::BINUNICODE:
                opcode_str = parseBinunicode();
                break;

            case OpCode::BINSTRING:
                opcode_str = parseBinstring();
                break;

            case OpCode::SHORT_BINSTRING:
                opcode_str = parseShortBinstring();
                break;

            case OpCode::BININT:
                opcode_str = parseBinint();
                break;

            case OpCode::BININT1:
                opcode_str = parseBinint1();
                break;

            case OpCode::BININT2:
                opcode_str = parseBinint2();
                break;

            case OpCode::BINGET:
                opcode_str = parseBinget();
                break;

            case OpCode::LONG_BINGET:
                opcode_str = parseLongBinget();
                break;

            case OpCode::BINPUT:
                opcode_str = parseBinput();
                break;

            case OpCode::LONG_BINPUT:
                opcode_str = parseLongBinput();
                break;

            case OpCode::BINPERSID:
                opcode_str = parseBinpersid();
                break;

            case OpCode::GET:
                opcode_str = parseGet();
                break;

            case OpCode::PUT:
                opcode_str = parsePut();
                break;

            case OpCode::BINBYTES:
                opcode_str = parseBinbytes();
                break;

            case OpCode::SHORT_BINBYTES:
                opcode_str = parseShortBinbytes();
                break;

            case OpCode::SHORT_BINUNICODE:
                opcode_str = parseShortBinunicode();
                break;

            case OpCode::BINUNICODE8:
                opcode_str = parseBinunicode8();
                break;

            case OpCode::BINBYTES8:
                opcode_str = parseBinbytes8();
                break;

            case OpCode::LONG1:
                opcode_str = parseLong1();
                break;

            case OpCode::LONG4:
                opcode_str = parseLong4();
                break;

            case OpCode::FRAME:
                opcode_str = parseFrame();
                break;

            case OpCode::MEMOIZE:
                opcode_str = parseMemoize();
                break;

            // no argument
            case OpCode::MARK:
            case OpCode::STOP:
            case OpCode::POP:
            case OpCode::POP_MARK:
            case OpCode::DUP:
            case OpCode::NONE:
            case OpCode::NEWFALSE:
            case OpCode::NEWTRUE:
            case OpCode::EMPTY_TUPLE:
            case OpCode::EMPTY_LIST:
            case OpCode::EMPTY_DICT:
            case OpCode::EMPTY_SET:
            case OpCode::APPEND:
            case OpCode::APPENDS:
            case OpCode::SETITEM:
            case OpCode::SETITEMS:
            case OpCode::ADDITEMS:
            case OpCode::BUILD:
            case OpCode::REDUCE:
            case OpCode::NEWOBJ:
            case OpCode::NEWOBJ_EX:
            case OpCode::TUPLE:
            case OpCode::TUPLE1:
            case OpCode::TUPLE2:
            case OpCode::TUPLE3:
            case OpCode::LIST:
            case OpCode::DICT:
            case OpCode::FROZENSET:
            case OpCode::STACK_GLOBAL:
                opcode_str = opCodeToString(opcode);
                break;

            default: {
                std::stringstream ss;
                ss << "UNKNOWN(0x" << std::hex << std::uppercase << std::setw(2)
                   << std::setfill('0') << static_cast<int>(opcode_byte) << ")";
                opcode_str = ss.str();
                break;
            }
        }

        opcodes.push_back(opcode_str);

        if (opcode == OpCode::STOP) {
            break;
        }
    }

    return opcodes;
}

uint8_t PickleVM::readByte() {
    if (pos_ >= data_.size()) {
        throw std::runtime_error("Unexpected end of pickle data");
    }
    return static_cast<uint8_t>(data_[pos_++]);
}

/**
 * @brief read byte for little endian
 */
uint16_t PickleVM::readUint16() {
    uint8_t b1 = readByte();
    uint8_t b2 = readByte();
    return b1 | (b2 << 8); 
}

/**
 * @brief read byte for little endian
 */
uint32_t PickleVM::readUint32() {
    uint8_t b1 = readByte();
    uint8_t b2 = readByte();
    uint8_t b3 = readByte();
    uint8_t b4 = readByte();
    return b1 | (b2 << 8) | (b3 << 16) | (b4 << 24); 
}

int32_t PickleVM::readInt32() {
    uint32_t val = readUint32();
    return static_cast<int32_t>(val);
}

std::string PickleVM::readLine() {
    std::string line;
    while (hasMore()) {
        char c = static_cast<char>(readByte());
        if (c == '\n') {
            break;
        }
        line += c;
    }
    return line;
}

std::string PickleVM::readBytes(size_t count) {
    std::string bytes;
    bytes.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        bytes += static_cast<char>(readByte());
    }
    return bytes;
}

std::string PickleVM::bytesToHex(const std::string& bytes) {
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0');
    for (unsigned char c : bytes) {
        ss << std::setw(2) << static_cast<int>(c);
    }
    return ss.str();
}

// Opcode parsers
std::string PickleVM::parseProto() {
    uint8_t version = readByte();
    return "PROTO(" + std::to_string(version) + ")";
}

std::string PickleVM::parseGlobal() {
    std::string module = readLine();
    std::string name = readLine();
    return "GLOBAL('" + module + "' '" + name + "')";
}

std::string PickleVM::parseBinunicode() {
    uint32_t length = readUint32();
    std::string str = readBytes(length);
    return "BINUNICODE('" + str + "')";
}

std::string PickleVM::parseBinstring() {
    uint32_t length = readUint32();
    std::string str = readBytes(length);
    return "BINSTRING(len=" + std::to_string(length) + ")";
}

std::string PickleVM::parseShortBinstring() {
    uint8_t length = readByte();
    std::string str = readBytes(length);
    return "SHORT_BINSTRING(len=" + std::to_string(length) + ")";
}

std::string PickleVM::parseBinint() {
    int32_t value = readInt32();
    return "BININT(" + std::to_string(value) + ")";
}

std::string PickleVM::parseBinint1() {
    uint8_t value = readByte();
    return "BININT1(" + std::to_string(value) + ")";
}

std::string PickleVM::parseBinint2() {
    uint16_t value = readUint16();
    return "BININT2(" + std::to_string(value) + ")";
}

std::string PickleVM::parseBinget() {
    uint8_t index = readByte();
    return "BINGET(" + std::to_string(index) + ")";
}

std::string PickleVM::parseLongBinget() {
    uint32_t index = readUint32();
    return "LONG_BINGET(" + std::to_string(index) + ")";
}

std::string PickleVM::parseBinput() {
    uint8_t index = readByte();
    return "BINPUT(" + std::to_string(index) + ")";
}

std::string PickleVM::parseLongBinput() {
    uint32_t index = readUint32();
    return "LONG_BINPUT(" + std::to_string(index) + ")";
}

std::string PickleVM::parseBinpersid() {
    return "BINPERSID";
}

std::string PickleVM::parseGet() {
    std::string index = readLine();
    return "GET(" + index + ")";
}

std::string PickleVM::parsePut() {
    std::string index = readLine();
    return "PUT(" + index + ")";
}

std::string PickleVM::parseBinbytes() {
    uint32_t length = readUint32();
    std::string bytes = readBytes(length);
    return "BINBYTES(len=" + std::to_string(length) + ")";
}

std::string PickleVM::parseShortBinbytes() {
    uint8_t length = readByte();
    std::string bytes = readBytes(length);
    return "SHORT_BINBYTES(len=" + std::to_string(length) + ")";
}

std::string PickleVM::parseShortBinunicode() {
    uint8_t length = readByte();
    std::string str = readBytes(length);
    return "SHORT_BINUNICODE('" + str + "')";
}

std::string PickleVM::parseBinunicode8() {
    // Read 8 bytes for length (uint64_t)
    uint64_t length = 0;
    for (int i = 0; i < 8; ++i) {
        length |= (static_cast<uint64_t>(readByte()) << (i * 8));
    }
    std::string str = readBytes(length);
    return "BINUNICODE8(len=" + std::to_string(length) + ")";
}

std::string PickleVM::parseBinbytes8() {
    // Read 8 bytes for length (uint64_t)
    uint64_t length = 0;
    for (int i = 0; i < 8; ++i) {
        length |= (static_cast<uint64_t>(readByte()) << (i * 8));
    }
    std::string bytes = readBytes(length);
    return "BINBYTES8(len=" + std::to_string(length) + ")";
}

std::string PickleVM::parseLong1() {
    uint8_t length = readByte();
    std::string bytes = readBytes(length);
    return "LONG1(len=" + std::to_string(length) + ")";
}

std::string PickleVM::parseLong4() {
    uint32_t length = readUint32();
    std::string bytes = readBytes(length);
    return "LONG4(len=" + std::to_string(length) + ")";
}

/**
 * @brief read 8 bytes for frame size (uint64_t)
 */
std::string PickleVM::parseFrame() {
    
    uint64_t frame_size = 0;
    for (int i = 0; i < 8; ++i) {
        frame_size |= (static_cast<uint64_t>(readByte()) << (i * 8));
    }
    return "FRAME(" + std::to_string(frame_size) + ")";
}

std::string PickleVM::parseMemoize() {
    return "MEMOIZE";
}

} // namespace vm
} // namespace tfe
