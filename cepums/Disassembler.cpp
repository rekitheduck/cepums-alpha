#include "Disassembler.h"

#include "Core.h"
#include "Log.h"
#include "Memory.h"

#include <fstream>
#include <iomanip>

#include "Processor/ProcessorUtils.h"

#define REG_A "r" << static_cast<uint32_t>(register_a)
#define REG_B "r" << static_cast<uint32_t>(register_b)
#define REG_C "r" << static_cast<uint32_t>(register_c)
#define MEM_DISPL std::hex << static_cast<int16_t>(memory_displacement) << std::dec << "h"
#define BRANCH_DISPL std::hex << static_cast<int16_t>(branch_displacement) << std::dec << "h"
#define INT_OPERATE_LIT std::hex << static_cast<int16_t>(integer_operate_literal) << std::dec << "h"
#define SIGNED_DISPL_10 std::hex << static_cast<int16_t>(signed_displ_10) << std::dec << "h"
#define HW_RET_DISPL_12 \
    std::hex << static_cast<int16_t>(EXTRACT_HW_RET_DISPL_12(instruction_doubleword)) << std::dec << "h"

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

        LOG_DEBUG("    instruction_doubleword 0b{0:b}", instruction_doubleword);
        LOG_DEBUG("    Register A: {0}  0b{0:b}", register_a);
        LOG_DEBUG("    Register B: {0}  0b{0:b}", register_b);
        LOG_DEBUG("    branch_displacement: {0:x}h  0b{0:b}", branch_displacement);
        LOG_DEBUG("    pal_function: {0:x}h  0b{0:b}", pal_function);
        LOG_DEBUG("    memory_displacement: {0:x}h  0b{0:b}", memory_displacement);
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
                if (instruction_doubleword == 0) {
                    output << std::endl;
                    break;
                }
                TODO();
                break;
            }
            case Instruction::LDA: {
                output << "lda " << REG_A << ", (" << MEM_DISPL << ") " << REG_B << std::endl;
                break;
            }
            case Instruction::LDAH:
                output << "ldah " << REG_A << ", (" << MEM_DISPL << ") " << REG_B << std::endl;
                break;
            case Instruction::BR:
            case Instruction::BSR: {
                output << "br " << REG_A << ", " << signExtendBranchDisplacementToQuad(branch_displacement)
                       << std::endl;
                break;
            }
            case Instruction::MTPR: {
                output << "mtpr " << REG_B << ", " << MEM_DISPL << std::endl;
                break;
            }
            case Instruction::MFPR: {
                output << "mfpr " << REG_A << ", " << MEM_DISPL << std::endl;
                break;
            }

            case Instruction::HW_RET: {
                switch (parseHW_RET_Hint(EXTRACT_HW_RET_HINT(instruction_doubleword))) {
                    case HW_RET_Hint::HW_JMP: output << "hw_jmp "; break;
                    case HW_RET_Hint::HW_JSR: output << "hw_jsr "; break;
                    case HW_RET_Hint::HW_RET: output << "hw_ret "; break;
                    case HW_RET_Hint::HW_COROUTINE: output << "hw_coroutine "; break;
                }
                output << REG_B << ", " << HW_RET_DISPL_12 << std::endl;
                break;
            }
            case Instruction::INTS: {
                // Integer Shift Instructions
                switch (integer_operate_opcode) {
                    case Instruction::SLL:
                        if (is_integer_operate_literal == 0) {
                            output << "sll " << REG_C << ", " << REG_A << ", " << REG_B << std::endl;
                        } else {
                            output << "sll " << REG_C << ", " REG_A << ", " << INT_OPERATE_LIT << std::endl;
                        }
                        break;
                    default: TODO_INSTRUCTION(instructionMnemonic(integer_operate_opcode), opcode); return;
                }
                break;
            }
            case Instruction::HW_LD: {
                const int16_t signed_displ_10 = EXTRACT_HW_LD_DISPL_10(instruction_doubleword);
                output << "hw_ld " << REG_A << ", " REG_B << ", " << SIGNED_DISPL_10 << std::endl;
                break;
            }
            case Instruction::INTL: {
                // Integer Logical Instructions
                switch (integer_operate_opcode) {
                    case Instruction::AND:
                        if (is_integer_operate_literal == 0) {
                            output << "and " << REG_C << ", " << REG_A << ", " << REG_B << std::endl;
                        } else {
                            output << "and " << REG_C << ", " REG_A << ", " << INT_OPERATE_LIT << std::endl;
                        }
                        break;
                    default: TODO_INSTRUCTION(instructionMnemonic(integer_operate_opcode), opcode); return;
                }
                break;
            }
            case Instruction::INTA: {
                // Integer Arithmetic Instructions
                switch (integer_operate_opcode) {
                    case Instruction::ADDQ:
                        if (is_integer_operate_literal == 0) {
                            output << "addq " << REG_C << ", " << REG_A << ", " << REG_B << std::endl;
                        } else {
                            output << "addq " << REG_C << ", " REG_A << ", " << INT_OPERATE_LIT << std::endl;
                        }
                        break;
                    default: TODO_INSTRUCTION(instructionMnemonic(integer_operate_opcode), opcode); return;
                }
                break;
            }
            case Instruction::INTM: {
                // Integer Multiply Instructions
                switch (integer_operate_opcode) {
                    case Instruction::MULL:
                        if (is_integer_operate_literal == 0) {
                            output << "mull " << REG_C << ", " << REG_A << ", " << REG_B << std::endl;
                        } else {
                            output << "mull " << REG_C << ", " REG_A << ", " << INT_OPERATE_LIT << std::endl;
                        }
                        break;
                    default: TODO_INSTRUCTION(instructionMnemonic(integer_operate_opcode), opcode); return;
                }
                break;
            }
            case Instruction::JSR: {
                output << "jsr " << REG_A << ", " << REG_B << std::endl;
                break;
            }
            case Instruction::BNE: {
                output << "bne " << REG_A << ", " << BRANCH_DISPL << std::endl;
                break;
            }
            case Instruction::STQ: {
                output << "stq " << REG_A << ", " << REG_B << ", " << MEM_DISPL << std::endl;
                break;
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
