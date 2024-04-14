

namespace Cepums {

class Register {
   public:
    Register() {}
    Register(uint8_t reg_byte){m_number = reg_byte};

    bool isInteger() const { return m_is_integer; }
    uint8_t num() const { return m_number; }

   private:
    bool m_is_integer{true};
    uint8_t m_number = 40;
};
} // namespace Cepums
