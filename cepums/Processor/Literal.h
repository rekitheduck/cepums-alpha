#pragma once

#include "Operand.h"

#include <cstdint>

namespace Cepums {

class Literal : public Operand {
   public:
    Literal(uint64_t value) { m_value = value; }

    virtual bool isRegister() const override { return false; }
    virtual uint64_t value([[maybe_unused]] Processor* processor) override { return m_value; }

   private:
    uint64_t m_value{0};
};
} // namespace Cepums
