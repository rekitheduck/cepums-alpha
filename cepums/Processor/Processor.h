#pragma once

#include <cstdint>

namespace Cepums {

class Processor {
   public:
    Processor() { reset(); }

    void reset();
    void execute();

   private:
    // Program Counter register
    uint64_t m_pc{0};

    // Lock registers
    uint64_t m_lock_flag_register{0};
    uint64_t m_locked_physical_address_register{0};

    // Processor Cycle Counter register
    uint32_t m_pcc_cnt{0}; // lower 32-bits, increments and overflows
    uint32_t m_pcc_off{0}; // OS-defined and used

    // Integer registers
    uint64_t m_r0{0};
    uint64_t m_r1{0};
    uint64_t m_r2{0};
    uint64_t m_r3{0};
    uint64_t m_r4{0};
    uint64_t m_r5{0};
    uint64_t m_r6{0};
    uint64_t m_r7{0};
    uint64_t m_r8{0};
    uint64_t m_r9{0};
    uint64_t m_r10{0};
    uint64_t m_r11{0};
    uint64_t m_r12{0};
    uint64_t m_r13{0};
    uint64_t m_r14{0};
    uint64_t m_r15{0};
    uint64_t m_r16{0};
    uint64_t m_r17{0};
    uint64_t m_r18{0};
    uint64_t m_r19{0};
    uint64_t m_r20{0};
    uint64_t m_r21{0};
    uint64_t m_r22{0};
    uint64_t m_r23{0};
    uint64_t m_r24{0};
    uint64_t m_r25{0};
    uint64_t m_r26{0};
    uint64_t m_r27{0};
    uint64_t m_r28{0};
    uint64_t m_r29{0};
    uint64_t m_r30{0};
    const uint64_t m_r31{0};

    // Floating-point registers
    double m_f0{0};
    double m_f1{0};
    double m_f2{0};
    double m_f3{0};
    double m_f4{0};
    double m_f5{0};
    double m_f6{0};
    double m_f7{0};
    double m_f8{0};
    double m_f9{0};
    double m_f10{0};
    double m_f11{0};
    double m_f12{0};
    double m_f13{0};
    double m_f14{0};
    double m_f15{0};
    double m_f16{0};
    double m_f17{0};
    double m_f18{0};
    double m_f19{0};
    double m_f20{0};
    double m_f21{0};
    double m_f22{0};
    double m_f23{0};
    double m_f24{0};
    double m_f25{0};
    double m_f26{0};
    double m_f27{0};
    double m_f28{0};
    double m_f29{0};
    double m_f30{0};
    const double m_f30{0};
};
} // namespace Cepums
