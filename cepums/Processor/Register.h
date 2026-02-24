#include <cstdint>

namespace Cepums {

class Register {
   public:
    Register() {}
    Register(uint8_t register_bits) { m_registerBits = m_registerBits; }

    bool isInteger() const { return m_is_integer; }
    uint8_t registerBits() const { return m_registerBits; }

   private:
    bool m_is_integer{true};
    uint64_t m_registerBits{40};
};
} // namespace Cepums
