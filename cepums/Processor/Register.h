#pragma once

#include "Operand.h"

namespace Cepums {

class Register : public Operand {
   public:
    Register(uint8_t register_bits) { m_registerBits = register_bits; }

    virtual bool isRegister() const override { return true; }
    uint8_t registerBits() const { return m_registerBits; }
    virtual uint64_t value(Processor* processor) override;

   private:
    uint64_t m_registerBits{40};
};
} // namespace Cepums
