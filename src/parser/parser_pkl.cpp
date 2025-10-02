#include "parser/parser_pkl.h"

#include <cstring>
#include <iostream>
#include <sstream>

#include "error/error.h"

namespace tfe {
namespace parser {

PickleParser::PickleParser() : pos_(0) {}

PickleParser::~PickleParser() {}

PickleParser::PickleValue PickleParser::parse(const std::vector<char>& data) {
  pos_ = 0;
  stack_.clear();
  memo_.clear();
  structure_.clear();

  try {
    parseOpcodes(data);
  } catch (const std::exception& e) {
    throw error::ParserException(error::PARSE_ERROR, "Pickle parse failed: " + std::string(e.what()));
  }

  if (stack_.empty()) {
    return nullptr;
  }
  return PickleValue();
}

PickleParser::PickleValue PickleParser::parse(const std::string& data) {
  std::vector<char> buffer(data.begin(), data.end());
  return parse(buffer);
}

std::string PickleParser::getStructure() const {
  return structure_;
}

void PickleParser::parseOpcodes(const std::vector<char>& data) {
  while (pos_ < data.size()) {
    uint8_t opcode = static_cast<uint8_t>(data[pos_++]);

    switch (static_cast<OpCode>(opcode)) {
      case OpCode::PROTO: {
        uint8_t version = static_cast<uint8_t>(data[pos_++]);
        structure_ += "PROTO(" + std::to_string(version) + ")\n";
        break;
      }

      case OpCode::GLOBAL:
        handleGlobal(data);
        break;

      case OpCode::BINPUT:
        handleBinPut(data);
        break;

      case OpCode::BINGET:
        handleBinGet(data);
        break;

      case OpCode::BININT:
        handleBinInt(data);
        break;

      case OpCode::BININT1: {
        uint8_t value = static_cast<uint8_t>(data[pos_++]);
        structure_ += "BININT1(" + std::to_string(value) + ")\n";
        auto val = std::make_shared<int64_t>(value);
        stack_.push_back(std::static_pointer_cast<void>(val));
        break;
      }

      case OpCode::SHORT_BINSTRING:
        handleBinString(data);
        break;

      case OpCode::SHORT_BINUNICODE:
      case OpCode::BINUNICODE:
        handleBinUnicode(data);
        break;

      case OpCode::NONE:
        structure_ += "NONE\n";
        stack_.push_back(nullptr);
        break;

      case OpCode::NEWTRUE: {
        structure_ += "NEWTRUE\n";
        auto val = std::make_shared<bool>(true);
        stack_.push_back(std::static_pointer_cast<void>(val));
        break;
      }

      case OpCode::NEWFALSE: {
        structure_ += "NEWFALSE\n";
        auto val_false = std::make_shared<bool>(false);
        stack_.push_back(std::static_pointer_cast<void>(val_false));
        break;
      }

      case OpCode::EMPTY_DICT:
        handleEmptyDict();
        break;

      case OpCode::EMPTY_LIST: {
        structure_ += "EMPTY_LIST\n";
        auto list = std::make_shared<std::vector<std::shared_ptr<void>>>();
        stack_.push_back(std::static_pointer_cast<void>(list));
        break;
      }

      case OpCode::MARK:
        structure_ += "MARK\n";
        stack_.push_back(nullptr);
        break;

      case OpCode::TUPLE:
        handleTuple(data);
        break;

      case OpCode::SETITEMS:
        handleSetItems();
        break;

      case OpCode::REDUCE:
        handleReduce();
        break;

      case OpCode::BUILD: {
        structure_ += "BUILD\n";
        if (stack_.size() >= 2) {
          stack_.pop_back();
        }
        break;
      }

      case OpCode::STOP:
        structure_ += "STOP\n";
        return;

      default:
        structure_ += "UNKNOWN(0x" + std::to_string(opcode) + ")\n";
        break;
    }
  }
}

int32_t PickleParser::readInt32(const std::vector<char>& data) {
  int32_t value;
  std::memcpy(&value, &data[pos_], sizeof(int32_t));
  pos_ += sizeof(int32_t);
  return value;
}

int16_t PickleParser::readInt16(const std::vector<char>& data) {
  int16_t value;
  std::memcpy(&value, &data[pos_], sizeof(int16_t));
  pos_ += sizeof(int16_t);
  return value;
}

double PickleParser::readFloat64(const std::vector<char>& data) {
  double value;
  std::memcpy(&value, &data[pos_], sizeof(double));
  pos_ += sizeof(double);
  return value;
}

std::string PickleParser::readString(const std::vector<char>& data, size_t length) {
  std::string str(data.begin() + pos_, data.begin() + pos_ + length);
  pos_ += length;
  return str;
}

void PickleParser::handleGlobal(const std::vector<char>& data) {
  std::string module;
  while (pos_ < data.size() && data[pos_] != '\n') {
    module += data[pos_++];
  }
  pos_++;

  std::string name;
  while (pos_ < data.size() && data[pos_] != '\n') {
    name += data[pos_++];
  }
  pos_++;

  structure_ += "GLOBAL(" + module + "." + name + ")\n";

  auto obj = std::make_shared<PickleObject>();
  obj->module = module;
  obj->name = name;
  stack_.push_back(std::static_pointer_cast<void>(obj));
}

void PickleParser::handleBinPut(const std::vector<char>& data) {
  uint8_t index = static_cast<uint8_t>(data[pos_++]);
  structure_ += "BINPUT(" + std::to_string(index) + ")\n";

  if (!stack_.empty()) {
    memo_[index] = stack_.back();
  }
}

void PickleParser::handleBinGet(const std::vector<char>& data) {
  uint8_t index = static_cast<uint8_t>(data[pos_++]);
  structure_ += "BINGET(" + std::to_string(index) + ")\n";

  if (memo_.find(index) != memo_.end()) {
    stack_.push_back(memo_[index]);
  }
}

void PickleParser::handleBinInt(const std::vector<char>& data) {
  int32_t value = readInt32(data);
  structure_ += "BININT(" + std::to_string(value) + ")\n";

  auto val = std::make_shared<int64_t>(value);
  stack_.push_back(std::static_pointer_cast<void>(val));
}

void PickleParser::handleBinString(const std::vector<char>& data) {
  uint8_t length = static_cast<uint8_t>(data[pos_++]);
  std::string str = readString(data, length);
  structure_ += "SHORT_BINSTRING('" + str + "')\n";

  auto val = std::make_shared<std::string>(str);
  stack_.push_back(std::static_pointer_cast<void>(val));
}

void PickleParser::handleBinUnicode(const std::vector<char>& data) {
  size_t length;
  if (static_cast<OpCode>(data[pos_ - 1]) == OpCode::SHORT_BINUNICODE) {
    length = static_cast<uint8_t>(data[pos_++]);
  } else {
    length = static_cast<uint32_t>(readInt32(data));
  }

  std::string str = readString(data, length);
  structure_ += "BINUNICODE('" + str + "')\n";

  auto val = std::make_shared<std::string>(str);
  stack_.push_back(std::static_pointer_cast<void>(val));
}

void PickleParser::handleReduce() {
  structure_ += "REDUCE\n";

  if (stack_.size() >= 2) {
    auto args = stack_.back();
    stack_.pop_back();
    auto callable = stack_.back();
    stack_.pop_back();

    auto obj = std::make_shared<PickleObject>();
    stack_.push_back(std::static_pointer_cast<void>(obj));
  }
}

void PickleParser::handleTuple(const std::vector<char>& data) {
  structure_ += "TUPLE\n";

  std::vector<std::shared_ptr<void>> items;
  while (!stack_.empty()) {
    auto item = stack_.back();
    stack_.pop_back();

    if (item == nullptr) { 
      break;
    }
    items.insert(items.begin(), item);
  }

  auto tuple = std::make_shared<std::vector<std::shared_ptr<void>>>(items);
  stack_.push_back(std::static_pointer_cast<void>(tuple));
}

void PickleParser::handleEmptyDict() {
  structure_ += "EMPTY_DICT\n";

  auto dict = std::make_shared<std::map<std::string, std::shared_ptr<void>>>();
  stack_.push_back(std::static_pointer_cast<void>(dict));
}

void PickleParser::handleSetItems() {
  structure_ += "SETITEMS\n";

  std::vector<std::shared_ptr<void>> items;
  while (!stack_.empty()) {
    auto item = stack_.back();
    stack_.pop_back();

    if (item == nullptr) {
      break;
    }
    items.insert(items.begin(), item);
  }

  if (!stack_.empty()) {
    stack_.pop_back();
  }

  auto dict = std::make_shared<std::map<std::string, std::shared_ptr<void>>>();
  stack_.push_back(std::static_pointer_cast<void>(dict));
}

}  // namespace parser
}  // namespace tfe
