#pragma once

#include <array>
#include <cstdint>

#include "Core.h"
#include "Memory.h"
#include "Register.h"

namespace Cepums {

class Processor {
   public:
    Processor() { reset(); }

    void reset();
    void execute(Memory& m);

    // Register getters
    uint64_t& at() { return m_gp_registers[28]; }
    uint64_t& gp() { return m_gp_registers[29]; }
    uint64_t& sp() { return m_gp_registers[30]; }
    uint64_t getIntegerRegisterValue(const Register& reg) const;
    double getFloatRegisterValue(const Register& reg) const;
    void setIntegerRegister(const Register& reg, const uint64_t& value);
    void setFloatRegister(const Register& reg, const double& value);

    // Instructions here!!! :3

    // Load instructions
    void ins$lda(Memory& m, Register destination, Register source, uint16_t memory_disp); // Address
    void ins$ldb(Memory& m, Register destination, Register source, uint64_t memory_disp); // Byte
    void ins$ldbu(Memory& m, Register destination, Register source, uint64_t memory_disp); // Unsigned byte
    void ins$ldw(Memory& m, Register destination, Register source, uint64_t memory_disp); // Word
    void ins$ldwu(Memory& m, Register destination, Register source, uint64_t memory_disp); // Unsigned word
    void ins$ldl(Memory& m, Register destination, Register source, uint64_t memory_disp); // Sign extended long
    void ins$ldl_l(Memory& m, Register destination, Register source,
                   uint64_t memory_disp); // Sign extended long locked (???)
    void ins$ldq(Memory& m, Register destination, Register source, uint64_t memory_disp); // Quad
    void ins$ldq_l(Memory& m, Register destination, Register source, uint64_t memory_disp); // Quad locked
    void ins$ldq_u(Memory& m, Register destination, Register source, uint64_t memory_disp); // Quad unaligned
    void ins$uldw(Memory& m, Register destination, Register source, uint64_t memory_disp); // Unaligned word
    void ins$uldwu(Memory& m, Register destination, Register source,
                   uint64_t memory_disp); // Unaligned word unsigned
    void ins$uldl(Memory& m, Register destination, Register source,
                  uint64_t memory_disp); // Unaligned load long ???? (bad docs)
    void ins$uldq(Memory& m, Register destination, Register source,
                  uint64_t memory_disp); // Unaligned load quad ??? (bad docs)

    void ins$ldah(Memory& m, Register destination, Register source, uint16_t memory_disp); // Address High
    void ins$ldgp(Memory& m, Register destination, Register source, uint64_t base_register); // GP / r29

    void ins$ldil(Memory& m, Register destination, uint64_t val_expr); // Immediate long
    void ins$ldiq(Memory& m, Register destination, uint64_t val_expr); // Immediate quad

    // Store instructions
    void ins$stb(Memory& m, uint64_t address_dest, Register source); // Byte
    void ins$stw(Memory& m, uint64_t address_dest, Register source); // Word
    void ins$stl(Memory& m, uint64_t address_dest, Register source); // Long
    void ins$stl_c(Memory& m, uint64_t address_dest, Register source); // Long conditional
    void ins$stq(Memory& m, uint64_t address_dest, Register source); // Quad
    void ins$stq_c(Memory& m, uint64_t address_dest, Register source); // Quad conditional
    void ins$stq_u(Memory& m, uint64_t address_dest, Register source); // Quad unaligned
    void ins$ustw(Memory& m, uint64_t address_dest, Register source); // Unaligned word
    void ins$ustl(Memory& m, uint64_t address_dest, Register source); // Unaligned long
    void ins$ustq(Memory& m, uint64_t address_dest, Register source); // Unaligned quad

    // Integer shift instructions (INTS)
    void ins$sll(Memory& m, Register input_one, Scope<Operand> input_two, Register output);

    // PAL
    void ins$call_pal(Memory& m, uint32_t function);
    void ins$mtpr(Memory& m, Register source, uint16_t index);

    // Branch instructions
    void ins$br(Memory& m, Register reg, uint32_t branch_displacement);

   private:
    // Program Counter register
    uint64_t m_pc{0xfffffc0000000000}; // reset address for PAL
    bool m_in_pal_mode{true}; // start in PAL mode

    // Lock registers
    uint64_t m_lock_flag_register{0};
    uint64_t m_locked_physical_address_register{0};

    // Internal Processor Registers (IPRs). Very PAL-specific
    uint64_t m_iprs[64];

    // Processor Cycle Counter register
    uint32_t m_pcc_cnt{0}; // lower 32-bits, increments and overflows
    uint32_t m_pcc_off{0}; // OS-defined and used

    // Integer registers
    std::array<uint64_t, 31> m_gp_registers{0}; // 31 integer registers

    // Floating-point registers
    std::array<double, 31> m_fp_registers{0.0}; // 31 floating point registers
};
} // namespace Cepums
