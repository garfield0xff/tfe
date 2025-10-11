/**
 * @brief Pickle opcodes definitions
 */

#ifndef TFE_VM_OP_PKL_H
#define TFE_VM_OP_PKL_H

#include <cstdint>
#include <cstring>
#include <sstream>
#include <string>
#include <string_view>
#include <iomanip>

namespace tfe {
namespace vm {



/**
 * @brief Single-byte pickle opcodes. See https://github.com/python/cpython/blob/main/Lib/pickletools.py, https://github.com/python/cpython/blob/main/Modules/_pickle.c
 */
enum class OpCode : uint8_t {
    MARK            = '(',
    STOP            = '.',
    POP             = '0',
    POP_MARK        = '1',
    DUP             = '2',
    FLOAT           = 'F',
    INT             = 'I',
    BININT          = 'J',
    BININT1         = 'K',
    LONG            = 'L',
    BININT2         = 'M',
    NONE            = 'N',
    PERSID          = 'P',
    BINPERSID       = 'Q',
    REDUCE          = 'R',
    STRING          = 'S',
    BINSTRING       = 'T',
    SHORT_BINSTRING = 'U',
    UNICODE         = 'V',
    BINUNICODE      = 'X',
    APPEND          = 'a',
    BUILD           = 'b',
    GLOBAL          = 'c',
    DICT            = 'd',
    EMPTY_DICT      = '}',
    APPENDS         = 'e',
    GET             = 'g',
    BINGET          = 'h',
    INST            = 'i',
    LONG_BINGET     = 'j',
    LIST            = 'l',
    EMPTY_LIST      = ']',
    OBJ             = 'o',
    PUT             = 'p',
    BINPUT          = 'q',
    LONG_BINPUT     = 'r',
    SETITEM         = 's',
    TUPLE           = 't',
    EMPTY_TUPLE     = ')',
    SETITEMS        = 'u',
    BINFLOAT        = 'G',

    /* Protocol 2. */
    PROTO       = 0x80,
    NEWOBJ      = 0x81,
    EXT1        = 0x82,
    EXT2        = 0x83,
    EXT4        = 0x84,
    TUPLE1      = 0x85,
    TUPLE2      = 0x86,
    TUPLE3      = 0x87,
    NEWTRUE     = 0x88,
    NEWFALSE    = 0x89,
    LONG1       = 0x8a,
    LONG4       = 0x8b,

    /* Protocol 3 (Python 3.x) */
    BINBYTES       = 'B',
    SHORT_BINBYTES = 'C',

    /* Protocol 4 */
    SHORT_BINUNICODE = 0x8c,
    BINUNICODE8      = 0x8d,
    BINBYTES8        = 0x8e,
    EMPTY_SET        = 0x8f,
    ADDITEMS         = 0x90,
    FROZENSET        = 0x91,
    NEWOBJ_EX        = 0x92,
    STACK_GLOBAL     = 0x93,
    MEMOIZE          = 0x94,
    FRAME            = 0x95,

    /* Protocol 5 */
    BYTEARRAY8       = 0x96,
    NEXT_BUFFER      = 0x97,
    READONLY_BUFFER  = 0x98
};

inline std::string opCodeToString(OpCode opcode) {
  switch (opcode) {
    case OpCode::MARK:
      return "MARK";
    case OpCode::STOP:
      return "STOP";
    case OpCode::POP:
      return "POP";
    case OpCode::POP_MARK:
      return "POP_MARK";
    case OpCode::DUP:
      return "DUP";
    case OpCode::FLOAT:
      return "FLOAT";
    case OpCode::INT:
      return "INT";
    case OpCode::BININT:
      return "BININT";
    case OpCode::BININT1:
      return "BININT1";
    case OpCode::LONG:
      return "LONG";
    case OpCode::BININT2:
      return "BININT2";
    case OpCode::NONE:
      return "NONE";
    case OpCode::PERSID:
      return "PERSID";
    case OpCode::BINPERSID:
      return "BINPERSID";
    case OpCode::REDUCE:
      return "REDUCE";
    case OpCode::STRING:
      return "STRING";
    case OpCode::BINSTRING:
      return "BINSTRING";
    case OpCode::SHORT_BINSTRING:
      return "SHORT_BINSTRING";
    case OpCode::UNICODE:
      return "UNICODE";
    case OpCode::BINUNICODE:
      return "BINUNICODE";
    case OpCode::APPEND:
      return "APPEND";
    case OpCode::BUILD:
      return "BUILD";
    case OpCode::GLOBAL:
      return "GLOBAL";
    case OpCode::DICT:
      return "DICT";
    case OpCode::EMPTY_DICT:
      return "EMPTY_DICT";
    case OpCode::APPENDS:
      return "APPENDS";
    case OpCode::GET:
      return "GET";
    case OpCode::BINGET:
      return "BINGET";
    case OpCode::INST:
      return "INST";
    case OpCode::LONG_BINGET:
      return "LONG_BINGET";
    case OpCode::LIST:
      return "LIST";
    case OpCode::EMPTY_LIST:
      return "EMPTY_LIST";
    case OpCode::OBJ:
      return "OBJ";
    case OpCode::PUT:
      return "PUT";
    case OpCode::BINPUT:
      return "BINPUT";
    case OpCode::LONG_BINPUT:
      return "LONG_BINPUT";
    case OpCode::SETITEM:
      return "SETITEM";
    case OpCode::TUPLE:
      return "TUPLE";
    case OpCode::EMPTY_TUPLE:
      return "EMPTY_TUPLE";
    case OpCode::SETITEMS:
      return "SETITEMS";
    case OpCode::BINFLOAT:
      return "BINFLOAT";

    // Protocol 2
    case OpCode::PROTO:
      return "PROTO";
    case OpCode::NEWOBJ:
      return "NEWOBJ";
    case OpCode::EXT1:
      return "EXT1";
    case OpCode::EXT2:
      return "EXT2";
    case OpCode::EXT4:
      return "EXT4";
    case OpCode::TUPLE1:
      return "TUPLE1";
    case OpCode::TUPLE2:
      return "TUPLE2";
    case OpCode::TUPLE3:
      return "TUPLE3";
    case OpCode::NEWTRUE:
      return "NEWTRUE";
    case OpCode::NEWFALSE:
      return "NEWFALSE";
    case OpCode::LONG1:
      return "LONG1";
    case OpCode::LONG4:
      return "LONG4";

    // Protocol 3
    case OpCode::BINBYTES:
      return "BINBYTES";
    case OpCode::SHORT_BINBYTES:
      return "SHORT_BINBYTES";

    // Protocol 4
    case OpCode::SHORT_BINUNICODE:
      return "SHORT_BINUNICODE";
    case OpCode::BINUNICODE8:
      return "BINUNICODE8";
    case OpCode::BINBYTES8:
      return "BINBYTES8";
    case OpCode::EMPTY_SET:
      return "EMPTY_SET";
    case OpCode::ADDITEMS:
      return "ADDITEMS";
    case OpCode::FROZENSET:
      return "FROZENSET";
    case OpCode::NEWOBJ_EX:
      return "NEWOBJ_EX";
    case OpCode::STACK_GLOBAL:
      return "STACK_GLOBAL";
    case OpCode::MEMOIZE:
      return "MEMOIZE";
    case OpCode::FRAME:
      return "FRAME";

    // Protocol 5
    case OpCode::BYTEARRAY8:
      return "BYTEARRAY8";
    case OpCode::NEXT_BUFFER:
      return "NEXT_BUFFER";
    case OpCode::READONLY_BUFFER:
      return "READONLY_BUFFER";

    default: {
      std::stringstream ss;
      ss << "UNKNOWN(0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
         << static_cast<int>(opcode) << ")";
      return ss.str();
    };
  }
}

}  // namespace vm
}  // namespace tfe

#endif  // TFE_VM_OP_PKL_H