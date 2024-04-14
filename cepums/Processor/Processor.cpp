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
    uint8_t opcode = EXTRACT_OPCODE(instruction_quad);

    // Memory Instruction Format (might get unused if the current instruction doesn't use this)
    uint8_t register_a = EXTRACT_REGISTER_A(instruction_quad);
    uint8_t register_b = EXTRACT_REGISTER_B(instruction_quad);
    uint16_t memory_displacement = EXTRACT_MEMORY_DISPLACEMENT(instruction_quad);

    switch (opcode) {
        case 0x00: return ins$call_pal(m, instruction_quad);
        case 0x08: return ins$lda(m, Cepums::Register(register_a), Cepums::Register(register_b), memory_displacement);
        default: ILLEGAL_INSTRUCTION(); break;
    }

    m_pc += 4;
}

uint64_t Processor::getIntegerRegister(const Register& reg) const {
    if (reg.isInteger() == false) {
        // Tried to get integer register with a float register value
        VERIFY_NOT_REACHED();
    }
    switch (reg.num()) {
        case 0: return m_r0;
        case 1: return m_r1;
        case 2: return m_r2;
        case 3: return m_r3;
        case 4: return m_r4;
        case 5: return m_r5;
        case 6: return m_r6;
        case 7: return m_r7;
        case 8: return m_r8;
        case 9: return m_r9;
        case 10: return m_r10;
        case 11: return m_r11;
        case 12: return m_r12;
        case 13: return m_r13;
        case 14: return m_r14;
        case 15: return m_r15;
        case 16: return m_r16;
        case 17: return m_r17;
        case 18: return m_r18;
        case 19: return m_r19;
        case 20: return m_r20;
        case 21: return m_r21;
        case 22: return m_r22;
        case 23: return m_r23;
        case 24: return m_r24;
        case 25: return m_r25;
        case 26: return m_r26;
        case 27: return m_r27;
        case 28: return m_r28;
        case 29: return m_r29;
        case 30: return m_r30;
        default: VERIFY_NOT_REACHED(); break;
    }

    return 0;
}

void Processor::setIntegerRegister(const Register& reg, uint64_t value) {
    if (reg.isInteger() == false) {
        // Tried to set integer register with a float register value
        VERIFY_NOT_REACHED();
    }
    switch (reg.num()) {
        case 0: m_r0 = value; break;
        case 1: m_r1 = value; break;
        case 2: m_r2 = value; break;
        case 3: m_r3 = value; break;
        case 4: m_r4 = value; break;
        case 5: m_r5 = value; break;
        case 6: m_r6 = value; break;
        case 7: m_r7 = value; break;
        case 8: m_r8 = value; break;
        case 9: m_r9 = value; break;
        case 10: m_r10 = value; break;
        case 11: m_r11 = value; break;
        case 12: m_r12 = value; break;
        case 13: m_r13 = value; break;
        case 14: m_r14 = value; break;
        case 15: m_r15 = value; break;
        case 16: m_r16 = value; break;
        case 17: m_r17 = value; break;
        case 18: m_r18 = value; break;
        case 19: m_r19 = value; break;
        case 20: m_r20 = value; break;
        case 21: m_r21 = value; break;
        case 22: m_r22 = value; break;
        case 23: m_r23 = value; break;
        case 24: m_r24 = value; break;
        case 25: m_r25 = value; break;
        case 26: m_r26 = value; break;
        case 27: m_r27 = value; break;
        case 28: m_r28 = value; break;
        case 29: m_r29 = value; break;
        case 30: m_r30 = value; break;
        default: VERIFY_NOT_REACHED(); break;
    }
}

void Processor::ins$lda(Memory& m, Register destination, Register source, uint16_t memory_disp) {
    int16_t signed_disp = memory_disp;
    uint64_t address = getIntegerRegister(source) + signed_disp;
    setIntegerRegister(destination, address);
}

void Processor::ins$ldah(Memory& m, Register destination, Register source, uint16_t memory_disp) {
    int16_t signed_disp = memory_disp * 65536;
    uint64_t address = getIntegerRegister(source) + signed_disp;
    setIntegerRegister(destination, address);
}

} // namespace Cepums
