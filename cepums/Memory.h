#pragma once

#include <cstdint>
#include <vector>

namespace Cepums {

struct Mapping {
    uint64_t start_address{0};
    bool read_only{true};
    std::vector<uint8_t> data;
};

class Memory {
   public:
    Memory();

    uint32_t readDouble(uint64_t address);
    uint64_t readQuad(uint64_t address);

    void mapROM(uint64_t address, std::vector<uint8_t> rom);

   private:
    std::vector<Mapping> m_mappings;
};
} // namespace Cepums
