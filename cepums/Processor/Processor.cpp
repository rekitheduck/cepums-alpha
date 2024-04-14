#include "Processor.h"
#include "cepumspch.h"

namespace Cepums {
void Processor::reset() {}

void Processor::execute(Memory& m) {
    LOG_DEBUG("PC: {0}", m_pc);

    // Fetch instruction (32-bits)
    uint32_t instruction_quad = m.readDouble(m_pc);

    // Decode instruction
    // 6-bit opcode
    // uint8_t opcode =

    m_pc += 4;
}

} // namespace Cepums
