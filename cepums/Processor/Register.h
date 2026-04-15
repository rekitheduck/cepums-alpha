#include <cstdint>

namespace Cepums {

class Register {
   public:
    Register(uint8_t register_bits) { m_registerBits = register_bits; }

    bool isInteger() const { return m_is_integer; }
    uint8_t registerBits() const { return m_registerBits; }
    uint64_t value();

   private:
    bool m_is_integer{true};
    uint64_t m_registerBits{40};
};
} // namespace Cepums
