#pragma once

#include <cstdint>
#include <vector>

namespace Cepums {

class Memory {
   public:
    Memory();

    uint32_t readDouble(uint64_t address);
    uint64_t readQuad(uint64_t address);

   private:
    std::vector<uint8_t> m_ram;
};
} // namespace Cepums
