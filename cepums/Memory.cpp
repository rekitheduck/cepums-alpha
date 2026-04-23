#include "Memory.h"
#include "Core.h"
#include "Log.h"

namespace Cepums {
Memory::Memory() {}

uint32_t Memory::readDouble(uint64_t address) {
    LOG_DEBUG("[MM] reading double at 0x{0:x}", address);
    for (const auto& mapping : m_mappings) {
        if (address >= mapping.start_address && address + 3 < mapping.start_address + mapping.data.size()) {
            const auto address_to_read = address - mapping.start_address;
            return (static_cast<uint32_t>(mapping.data[address_to_read])) | // LSB
                   (static_cast<uint32_t>(mapping.data[address_to_read + 1]) << 8) |
                   (static_cast<uint32_t>(mapping.data[address_to_read + 2]) << 16) |
                   (static_cast<uint32_t>(mapping.data[address_to_read + 3]) << 24);
        }
    }

    LOG_ERROR("[MM] Address={0:x}h not mapped!", address);
    VERIFY_NOT_REACHED();

    return 0;
}

uint64_t Memory::readQuad(uint64_t address) {
    LOG_DEBUG("[MM] reading quad at 0x{0:x}", address);

    for (const auto& mapping : m_mappings) {
        if (address >= mapping.start_address && address + 7 < mapping.start_address + mapping.data.size()) {
            const auto address_to_read = address - mapping.start_address;
            return (static_cast<uint64_t>(mapping.data[address_to_read])) | // LSB
                   (static_cast<uint64_t>(mapping.data[address_to_read + 1]) << 8) |
                   (static_cast<uint64_t>(mapping.data[address_to_read + 2]) << 16) |
                   (static_cast<uint64_t>(mapping.data[address_to_read + 3]) << 24) |
                   (static_cast<uint64_t>(mapping.data[address_to_read + 4]) << 32) |
                   (static_cast<uint64_t>(mapping.data[address_to_read + 5]) << 40) |
                   (static_cast<uint64_t>(mapping.data[address_to_read + 6]) << 48) |
                   (static_cast<uint64_t>(mapping.data[address_to_read + 7]) << 56);
        }
    }

    LOG_ERROR("[MM] Address={0:x}h not mapped!", address);
    VERIFY_NOT_REACHED();

    return 0;
}

void Memory::mapROM(uint64_t address, std::vector<uint8_t> rom) {
    // TODO: verify there's no overlap!
    m_mappings.emplace_back(address, true, rom);
}

} // namespace Cepums
