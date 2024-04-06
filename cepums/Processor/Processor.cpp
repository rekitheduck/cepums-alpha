#include "Processor.h"
#include "cepumspch.h"

namespace Cepums {
void Processor::reset() {}

void Processor::execute(Memory& m) {
    LOG_DEBUG("PC: {0}", m_pc);

    m_pc += 4;
}

} // namespace Cepums
