#include "vm/vm_pkl.h"

namespace tfe {
namespace vm {

PickleVM::PickleVM(): pos_(0) {}

void PickleVM::parseOpcodes(const std::vector<char>& data) {
    while(pos_ < data.size()) {
        uint8_t opcode = static_cast<uint8_t>(data[pos_++]);

        switch (static_cast<OpCode>(opcode))
        {
        case OpCode::MARK:
            break;
        case OpCode::STOP:
            break;
        case OpCode::POP:
            break;
        case OpCode::POP_MARK:
            break;
        case OpCode::DUP:
            break;
        case OpCode::FLOAT:
            break;
        case OpCode::INT:
            break;
        case OpCode::BININT:
            break;
        case OpCode::BININT1:
            break;
        case OpCode::LONG:
            break;
        case OpCode::BININT2:
            break;
        case OpCode::NONE:
            break;
        case OpCode::PERSID:
            break;
        case OpCode::BINPERSID:
            break;
        case OpCode::REDUCE:
            break;
        case OpCode::STRING:
            break;
        case OpCode::BINSTRING:
            break;
        case OpCode::SHORT_BINSTRING:
            break;
        case OpCode::UNICODE:
            break;
        case OpCode::BINUNICODE:
            break;
        case OpCode::APPEND:
            break;
        case OpCode::BUILD:
            break;
        case OpCode::GLOBAL:
            break;
        case OpCode::DICT:
            break;
        case OpCode::EMPTY_DICT:
            break;
        case OpCode::APPENDS:
            break;
        case OpCode::GET:
            break;
        case OpCode::BINGET:
            break;
        case OpCode::INST:
            break;
        case OpCode::LONG_BINGET:
            break;
        case OpCode::LIST:
            break;
        case OpCode::EMPTY_LIST:
            break;
        case OpCode::OBJ:
            break;
        case OpCode::PUT:
            break;
        case OpCode::BINPUT:
            break;
        case OpCode::LONG_BINPUT:
            break;
        case OpCode::SETITEM:
            break;
        case OpCode::TUPLE:
            break;
        case OpCode::EMPTY_TUPLE:
            break;
        case OpCode::SETITEMS:
            break;
        case OpCode::BINFLOAT:
            break;
        default:
            break;
        }
    }
}
}
}