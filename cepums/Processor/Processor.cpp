#include "Processor.h"

#include "Core.h"
#include "Log.h"
#include "ProcessorUtils.h"

#include "Literal.h"

namespace Cepums {
void Processor::reset() {}

void Processor::execute(Memory& m) {
    LOG_DEBUG("PC: {0:x}h", m_pc);

    // Fetch instruction (32-bits)
    uint32_t instruction_doubleword = m.readDouble(m_pc);
    LOG_DEBUG("Instruction data loaded: {0:x}h  0b{0:b}", instruction_doubleword);

    // Increment program counter
    m_pc += 4;

    // Decode various bits of the doubleword
    uint8_t opcode = EXTRACT_OPCODE(instruction_doubleword); // 6 bit opcode
    uint8_t register_a = EXTRACT_REGISTER_A(instruction_doubleword);
    uint8_t register_b = EXTRACT_REGISTER_B(instruction_doubleword);
    uint8_t register_c = EXTRACT_REGISTER_C(instruction_doubleword);
    uint16_t integer_operate_function = EXTRACT_INTEGER_OPERATE_FUNCTION(instruction_doubleword);
    uint16_t integer_operate_literal = EXTRACT_INTEGER_OPERATE_LITERAL(instruction_doubleword);
    uint8_t is_integer_operate_literal = EXTRACT_INTEGER_OPERATE_IS_LITERAL_BIT(instruction_doubleword);
    uint16_t memory_displacement = EXTRACT_MEMORY_DISPLACEMENT(instruction_doubleword);
    uint32_t branch_displacement = EXTRACT_BRANCH_DISPLACEMENT(instruction_doubleword);
    uint32_t pal_function = EXTRACT_PAL_FUNCTION(instruction_doubleword);

    LOG_DEBUG("    Register A: {0}  0b{0:b}", register_a);
    LOG_DEBUG("    Register B: {0}  0b{0:b}", register_b);
    LOG_DEBUG("    branch_displacement: {0:x}h  0b{0:b}", branch_displacement);
    LOG_DEBUG("    pal_function: {0:x}h  0b{0:b}", pal_function);
    LOG_DEBUG("    integer_operate_function: {0:x}h ", integer_operate_function);
    LOG_DEBUG("    integer_operate_literal: {0:x}h ", integer_operate_literal);

    // TODO: wikipedia has different bits defined, need to update them here and the above macros

    // Operate format uses:
    // opcode, RA, RB, integer_operate_function, RC

    // Memory format uses:
    // opcode, RA, RB, Memory displacement

    // Branch format uses:
    // opcode, RA, Branch displacement

    // PALcode format uses:
    // opcode, PAL function

    LOG_DEBUG("opcode: {0:x}h", opcode);
    const auto instruction = decodeInstruction(opcode);

    // opcode is the first 6 bits, so it can hold 64 distinct values which means 64 instructions
    switch (instruction) {
        case Instruction::CallPal: return ins$call_pal(m, pal_function);
        case Instruction::LDA:
            return ins$lda(m, Cepums::Register(register_a), Cepums::Register(register_b), memory_displacement);
        case Instruction::LDAH:
            return ins$ldah(m, Cepums::Register(register_a), Cepums::Register(register_b), memory_displacement);
        case Instruction::BR: return ins$br(m, Cepums::Register(register_a), branch_displacement);
        case Instruction::MTPR:
            return ins$mtpr(m, Cepums::Register(register_b),
                            memory_displacement); // register_a and register_b must be the same? but they're not for me
        case Instruction::INTS: {
            // Integer Shift Instructions
            const auto integer_operate_opcode = decodeFunctionedInstruction(instruction, integer_operate_function);
            switch (integer_operate_opcode) {
                case Instruction::SLL:
                    if (is_integer_operate_literal == 0) {
                        return ins$sll(m, Cepums::Register(register_a), createScope<Cepums::Register>(register_b),
                                       Cepums::Register(register_c));
                    } else {
                        return ins$sll(m, Cepums::Register(register_a),
                                       createScope<Cepums::Literal>(integer_operate_literal),
                                       Cepums::Register(register_c));
                    }
                default: TODO_INSTRUCTION(instructionMnemonic(integer_operate_opcode), opcode); return;
            }
        }
        case Instruction::Invalid: ILLEGAL_INSTRUCTION(); return;
        default: TODO_INSTRUCTION(instructionMnemonic(instruction), opcode); return;
    }
}

uint64_t Processor::getIntegerRegisterValue(const Register& reg) const {
    // Bad type
    if (reg.isInteger() == false) {
        LOG_DEBUG("getIntegerRegisterValue: bits: {0}", reg.registerBits());
        VERIFY_NOT_REACHED();
    }

    LOG_DEBUG("reg_bits: {0:b}", reg.registerBits());

    // We have 32 registers. Last one is hardwired to 0
    if (reg.registerBits() < 31) {
        return m_gp_registers[reg.registerBits()];
    } else if (reg.registerBits() == 31) {
        return 0;
    }
    LOG_DEBUG("getIntegerRegisterValue: bits: {0}", reg.registerBits());

    VERIFY_NOT_REACHED();

    return 0;
}

double Processor::getFloatRegisterValue(const Register& reg) const {
    // Bad type
    if (reg.isInteger() == true) {
        LOG_DEBUG("getIntegerRegisterValue: bits: {0}", reg.registerBits());
        VERIFY_NOT_REACHED();
    }

    // We have 32 registers. Last one is hardwired to 0
    if (reg.registerBits() < 31) {
        LOG_DEBUG("getIntegerRegisterValue: bits: {0}", reg.registerBits());
        return m_fp_registers[reg.registerBits()];
    } else if (reg.registerBits() == 31) {
        return 0.0;
    }

    VERIFY_NOT_REACHED();

    return 0;
}

void Processor::setIntegerRegister(const Register& reg, const uint64_t& value) {
    // Bad type
    if (reg.isInteger() == false) {
        VERIFY_NOT_REACHED();
    }

    // We have 32 registers. Writes to last one are ignored
    if (reg.registerBits() < 31) {
        m_gp_registers[reg.registerBits()] = value;
        return;
    } else if (reg.registerBits() == 31) {
        return;
    }

    VERIFY_NOT_REACHED();
}

void Processor::setFloatRegister(const Register& reg, const double& value) {
    // Bad type
    if (reg.isInteger() == true) {
        VERIFY_NOT_REACHED();
    }

    // We have 32 registers. Writes to last one are ignored
    if (reg.registerBits() < 31) {
        m_fp_registers[reg.registerBits()] = value;
        return;
    } else if (reg.registerBits() == 31) {
        return;
    }

    VERIFY_NOT_REACHED();
}

void Processor::ins$lda(Memory& m, Register destination, Register source, uint16_t memory_disp) {
    LOG_INFO("ins$lda");
    int16_t signed_disp = memory_disp;
    uint64_t address = getIntegerRegisterValue(source) + signed_disp;
    setIntegerRegister(destination, address);
}

void Processor::ins$ldah(Memory& m, Register destination, Register source, uint16_t memory_disp) {
    LOG_INFO("ins$ldah");
    int16_t signed_disp = memory_disp * 65536;
    uint64_t address = getIntegerRegisterValue(source) + signed_disp;
    setIntegerRegister(destination, address);
}

void Processor::ins$sll(Memory& m, Register input_one, Scope<Operand> input_two, Register output) {
    if (input_two->isRegister()) {
        LOG_INFO("ins$sll r{0}, r{1}, r{2}", input_one.registerBits(),
                 static_cast<Register*>(input_two.get())->registerBits(), output.registerBits());
    } else {
        LOG_INFO("ins$sll r{0}, {1}h, r{2}", input_one.registerBits(), input_two->value(this), output.registerBits());
    }

    m_gp_registers[output.registerBits()] = input_one.value(this) << input_two->value(this);
}

void Processor::ins$call_pal(Memory& m, uint32_t function) {
    LOG_INFO("ins$call_pal");
    LOG_DEBUG("[call_pal]    ins$call_pal with function {0:x}h", function);
    TODO();
}

void Processor::ins$br(Memory& m, Register reg, uint32_t branch_displacement) {
    LOG_INFO("ins$br");
    LOG_DEBUG("[br]    branch_displacement: {0:x}h", branch_displacement);
    LOG_DEBUG("[br]    reg value: {0:x}h", getIntegerRegisterValue(reg));
    LOG_DEBUG("[br]    gonna modify reg: {0}", reg.registerBits());
    setIntegerRegister(reg, m_pc);
    const auto new_displ = signExtendBranchDisplacementToQuad(branch_displacement);
    LOG_DEBUG("[br]    old PC: {0:x}h", m_pc);
    m_pc = m_pc + 4 * new_displ;
    LOG_DEBUG("[br]    new PC: {0:x}h", m_pc);
}

void Processor::ins$mtpr(Memory& m, Register source, uint16_t index) {
    LOG_INFO("ins$mtpr r{0}, {1}", source.registerBits(), index);
    LOG_DEBUG("[mtpr]    index: {0}", index);
    switch (index) {
        case 0: // Processor State flag
        case 50: m_iprs[index] = getIntegerRegisterValue(source); break;
        default: TODO(); break;
    }
}

} // namespace Cepums
