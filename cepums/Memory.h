#pragma once

#include <cstdint>

namespace Cepums {

class Memory {
   public:
    Memory();

    uint64_t readQuad(uint64_t address);

   private:
    std::vector<uint8_t> m_ram;
};
} // namespace Cepums
