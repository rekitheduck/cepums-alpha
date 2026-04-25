#pragma once

#include <cstdint>

namespace Cepums {

class Processor;

class Operand {
   public:
    bool isInteger() const { return m_is_integer; }
    virtual bool isRegister() const = 0;
    virtual uint64_t value(Processor* processor) = 0;

   private:
    bool m_is_integer{true};
};
} // namespace Cepums
