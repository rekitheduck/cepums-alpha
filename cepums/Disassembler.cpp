#include "Disassembler.h"

#include "Core.h"
#include "Log.h"
#include "Memory.h"

#include <fstream>
#include <iomanip>

#include "Processor/ProcessorUtils.h"

namespace Cepums {

namespace Disassembler {

void disassembleBinary(const std::vector<uint8_t>& blob, const uint64_t origin_address,
                       const std::string& output_filename, bool do_output_listing) {
    Cepums::Memory m;
    m.mapROM(origin_address, blob);

    std::ofstream output(output_filename, std::ios::binary);

    for (uint64_t pc = origin_address; pc < blob.size() + origin_address; pc += 4) {
        uint32_t instruction_doubleword = m.readDouble(pc);

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
        const auto instruction = decodeInstruction(opcode);

        const auto integer_operate_opcode = decodeFunctionedInstruction(instruction, integer_operate_function);

        // Output the address
        output << std::setfill('0') << std::setw(16) << std::hex << pc << "  ";

        // Output the hex instruction doubleword and put back into decimal mode
        output << std::setw(8) << instruction_doubleword << std::dec;

        // and prepare for the disassembly
        output << "          "; // 10 spaces

        // opcode is the first 6 bits, so it can hold 64 distinct values which means 64 instructions
        switch (instruction) {
            case Instruction::CallPal: {
                TODO();
                break;
            }
            case Instruction::LDA: {
                output << "lda r" << register_a << ", (" << std::hex << static_cast<int16_t>(memory_displacement)
                       << std::dec << "h)r" << register_b << std::endl;
                break;
            }
            case Instruction::LDAH:
                TODO();
                // return ins$ldah(m, Cepums::Register(register_a), Cepums::Register(register_b), memory_displacement);
            case Instruction::BR:
            case Instruction::BSR:
                output << "br r" << register_a << ", " << signExtendBranchDisplacementToQuad(branch_displacement)
                       << std::endl;
                TODO();
                // return ins$br(m, Cepums::Register(register_a), branch_displacement);

            case Instruction::MTPR: TODO();

            //     return ins$mtpr(m, Cepums::Register(register_b),
            //                     memory_displacement); // TODO: verify register a is 31 !!!
            // case Instruction::MFPR: TODO(); return ins$mfpr(m, Cepums::Register(register_a), memory_displacement);
            case Instruction::HW_RET: {
                TODO();

                // return ins$hw_ret(m, Cepums::Register(register_b),
                //                   Cepums::Literal(EXTRACT_HW_RET_DISPL_12(instruction_doubleword)),
                //                   parseHW_RET_Hint(EXTRACT_HW_RET_HINT(instruction_doubleword)),
                //                   EXTRACT_HW_RET_STALL_BIT(instruction_doubleword));
            }
            case Instruction::INTS: {
                TODO();

                // Integer Shift Instructions
                // switch (integer_operate_opcode) {
                //     case Instruction::SLL:
                //         if (is_integer_operate_literal == 0) {
                //             return ins$sll(m, Cepums::Register(register_a),
                //             createScope<Cepums::Register>(register_b),
                //                            Cepums::Register(register_c));
                //         } else {
                //             return ins$sll(m, Cepums::Register(register_a),
                //                            createScope<Cepums::Literal>(integer_operate_literal),
                //                            Cepums::Register(register_c));
                //         }
                //     default: TODO_INSTRUCTION(instructionMnemonic(integer_operate_opcode), opcode); return;
                // }
            }
            case Instruction::HW_LD: {
                TODO();

                // const int16_t signed_displ_10 = EXTRACT_HW_LD_DISPL_10(instruction_doubleword);
                // return ins$hw_ld(
                //     m, Cepums::Register(register_a), Cepums::Register(register_b), Cepums::Literal(signed_displ_10),
                //     Cepums::HW_LD_Flags(
                //         EXTRACT_HW_LD_LOCK_BIT(instruction_doubleword),
                //         EXTRACT_HW_LD_VPTE_BIT(instruction_doubleword),
                //         EXTRACT_HW_LD_QUAD_BIT(instruction_doubleword),
                //         EXTRACT_HW_LD_WRTCK_BIT(instruction_doubleword),
                //         EXTRACT_HW_LD_ALT_BIT(instruction_doubleword),
                //         EXTRACT_HW_LD_PHYS_BIT(instruction_doubleword)));
            }
            case Instruction::INTL: {
                TODO();

                // Integer Logical Instructions
                // switch (integer_operate_opcode) {
                //     case Instruction::AND:
                //         if (is_integer_operate_literal == 0) {
                //             return ins$and(m, Cepums::Register(register_a),
                //             createScope<Cepums::Register>(register_b),
                //                            Cepums::Register(register_c));
                //         } else {
                //             return ins$and(m, Cepums::Register(register_a),
                //                            createScope<Cepums::Literal>(integer_operate_literal),
                //                            Cepums::Register(register_c));
                //         }
                //     default: TODO_INSTRUCTION(instructionMnemonic(integer_operate_opcode), opcode); return;
                // }
            }
            case Instruction::INTA: {
                TODO();

                // Integer Arithmetic Instructions
                // switch (integer_operate_opcode) {
                //     case Instruction::ADDQ:
                //         if (is_integer_operate_literal == 0) {
                //             return ins$addq(m, Cepums::Register(register_a),
                //             createScope<Cepums::Register>(register_b),
                //                             Cepums::Register(register_c));
                //         } else {
                //             return ins$addq(m, Cepums::Register(register_a),
                //                             createScope<Cepums::Literal>(integer_operate_literal),
                //                             Cepums::Register(register_c));
                //         }
                //     default: TODO_INSTRUCTION(instructionMnemonic(integer_operate_opcode), opcode); return;
                // }
            }
            case Instruction::INTM: {
                TODO();

                // Integer Multiply Instructions
                // switch (integer_operate_opcode) {
                //     case Instruction::MULL:
                //         if (is_integer_operate_literal == 0) {
                //             return ins$mull(m, Cepums::Register(register_a),
                //             createScope<Cepums::Register>(register_b),
                //                             Cepums::Register(register_c));
                //         } else {
                //             return ins$mull(m, Cepums::Register(register_a),
                //                             createScope<Cepums::Literal>(integer_operate_literal),
                //                             Cepums::Register(register_c));
                //         }
                //     default: TODO_INSTRUCTION(instructionMnemonic(integer_operate_opcode), opcode); return;
                // }
            }
            case Instruction::JSR: {
                TODO();

                // TODO: use type hints to at least log the correct used mnemonic
                // return ins$jmp(m, Cepums::Register(register_a), Cepums::Register(register_b));
            }
            case Instruction::BNE: {
                TODO();

                // return ins$bne(m, Cepums::Register(register_a), Literal(branch_displacement));
            }
            case Instruction::STQ: {
                TODO();

                // return ins$stq(m, Cepums::Register(register_a), Cepums::Register(register_b),
                //                Literal(memory_displacement));
            }
            case Instruction::Invalid: ILLEGAL_INSTRUCTION(); return;
            default:
                // TODO: This might be easier in the future but need to differentiate sub function instructions in the
                // logging

                // if (integer_operate_opcode == Instruction::Invalid) {
                TODO_INSTRUCTION(instructionMnemonic(instruction), opcode);
                // } else {
                // TODO_INSTRUCTION(instructionMnemonic(integer_operate_opcode), opcode);
                // }
                break;
        };
    }
}
} // namespace Disassembler
} // namespace Cepums
