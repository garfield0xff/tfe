/**
 * @brief Pickle opcodes definitions
 */

#ifndef TFE_VM_OP_PKL_H
#define TFE_VM_OP_PKL_H

#include <cstdint>
#include <cstring>
#include <string_view>

namespace tfe {
namespace vm {

// Single-byte pickle opcodes. See https://github.com/python/cpython/blob/main/Lib/pickletools.py for extensive docs.
enum class OpCode : uint8_t {
    MARK            = '(',  // push special markobject on stack
    STOP            = '.',  // every pickle ends with STOP
    POP             = '0',  // discard topmost stack item
    POP_MARK        = '1',  // discard stack top through topmost markobject
    DUP             = '2',  // duplicate top stack item
    FLOAT           = 'F',  // push float object; decimal string argument
    INT             = 'I',  // push integer or bool; decimal string argument; TRUE(I01) OR FALSE(I00)
    BININT          = 'J',  // push four-byte signed int
    BININT1         = 'K',  // push 1-byte unsigned int
    LONG            = 'L',  // push long; decimal string argument
    BININT2         = 'M',  // push 2-byte unsigned int
    NONE            = 'N',  // push None
    PERSID          = 'P',  // push persistent object; id is taken from string arg
    BINPERSID       = 'Q',  // push persistent object; id is taken from stack
    REDUCE          = 'R',  // apply callable to argtuple, both on stack
    STRING          = 'S',  // push string; NL-terminated string argument
    BINSTRING       = 'T',  // push string; counted binary string argument
    SHORT_BINSTRING = 'U',  // push string; counted binary string argument < 256 bytes
    UNICODE         = 'V',  // push Unicode string; raw-unicode-escaped'd argument
    BINUNICODE      = 'X',  // push Unicode string; counted UTF-8 string argument
    APPEND          = 'a',  // append stack top to list below it
    BUILD           = 'b',  // call __setstate__ or __dict__.update()
    GLOBAL          = 'c',  // push self.find_class(modname, name); 2 string args
    DICT            = 'd',  // build a dict from stack items
    EMPTY_DICT      = '}',  // push empty dict
    APPENDS         = 'e',  // extend list on stack by topmost stack slice
    GET             = 'g',  // push item from memo on stack; index is string arg
    BINGET          = 'h',  // push item from memo on stack; index is 1-byte arg
    INST            = 'i',  // build & push class instance
    LONG_BINGET     = 'j',  // push item from memo on stack; index is 4-byte arg
    LIST            = 'l',  // build list from topmost stack items
    EMPTY_LIST      = ']',  // push empty list
    OBJ             = 'o',  // build & push class instance
    PUT             = 'p',  // store stack top in memo; index is string arg
    BINPUT          = 'q',  // store stack top in memo; index is 1-byte arg
    LONG_BINPUT     = 'r',  // store stack top in memo; index is 4-byte arg
    SETITEM         = 's',  // add key+value pair to dict
    TUPLE           = 't',  // build tuple from topmost stack items
    EMPTY_TUPLE     = ')',  // push empty tuple
    SETITEMS        = 'u',  // modify dict by adding topmost key+value pairs
    BINFLOAT        = 'G',  // push float; arg is 8-byte float encoding
};




} // namespace vm
} // namespace tfe

#endif // TFE_VM_OP_PKL_H