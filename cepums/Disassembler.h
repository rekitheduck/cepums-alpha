#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Cepums {

namespace Disassembler {

void disassembleBinary(const std::vector<uint8_t>& blob, const uint64_t origin_address,
                       const std::string& output_filename, bool do_output_listing);

} // namespace Disassembler
} // namespace Cepums
