#include "ProcessorUtils.h"

#include "Core.h"
#include "Log.h"

namespace Cepums {

Instruction decodeInstruction(uint8_t opcode) {
    // opcode is the first 6 bits, so it can hold 64 distinct values
    switch (opcode) {
        case 0x00: return Instruction::CallPal;
        case 0x08: return Instruction::LDA;
        case 0x09: return Instruction::LDAH;
    }
    VERIFY_NOT_REACHED();

    return Instruction::Invalid;
}

}; // namespace Cepums