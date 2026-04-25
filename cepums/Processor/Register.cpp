#include "Register.h"

#include "Processor.h"

namespace Cepums {

uint64_t Register::value(Processor* processor) {
    return processor->getIntegerRegisterValue(m_registerBits);
}

}; // namespace Cepums
