/**
 * @brief Pickle virtual machine for parsing torch script
 */

#ifndef TFE_VM_VM_PKL_H
#define TFE_VM_VM_PKL_H

#include "vm/op_pkl.h"
#include <vector>

namespace tfe {
namespace vm {
class PickleVM {
public:
 PickleVM();
 ~PickleVM();

  void parseOpcodes(const std::vector<char>& data);
  int32_t readInt32(const std::vector<char>& data);
  uint32_t readUInt32(const std::vector<char>& data);
  int16_t readInt16(const std::vector<char>& data);
  uint16_t readUInt16(const std::vector<char>& data);
  uint8_t readUInt8(const std::vector<char>& data);
  double readFloat64(const std::vector<char>& data);
private:
    size_t pos_;
};

} // namespace vm
} // namespace tfe

#endif // TFE_VM_VM_PKL_H