#pragma once

#include <cstdint>

namespace Cepums {

enum class Instruction { Invalid, CallPal, LDA, LDAH, BR, MTPR };

Instruction decodeInstruction(uint8_t opcode);

} // namespace Cepums
