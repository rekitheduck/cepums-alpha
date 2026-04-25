#pragma once

#include <memory>

#ifdef _WIN32
#ifdef CEPUMS_DEBUG
#define CEPUMS_ASSERT(x, ...)                                     \
    {                                                             \
        if (!(x)) {                                               \
            LOG_CRITICAL("Assertation failed: {0}", __VA_ARGS__); \
            __debugbreak();                                       \
        }                                                         \
    }
#else
#define CEPUMS_ASSERT(x, ...)
#endif
#define TODO()                                                   \
    {                                                            \
        LOG_CRITICAL("TODO hit in {0}:{1}", __FILE__, __LINE__); \
        __debugbreak();                                          \
    }
#define ILLEGAL_INSTRUCTION()                                                       \
    {                                                                               \
        LOG_CRITICAL("ILLEGAL INSTRUCTION REACHED in {0}:{1}", __FILE__, __LINE__); \
        __debugbreak();                                                             \
    }
#define UNKNOWN_INSTRUCTION()                                                      \
    {                                                                              \
        LOG_CRITICAL("Unknown instruction parsed in {0}:{1}", __FILE__, __LINE__); \
        __debugbreak();                                                            \
    }
#define VERIFY_NOT_REACHED()                                                \
    LOG_CRITICAL("Verify not reached hit in {0}:{1}!", __FILE__, __LINE__); \
    __debugbreak()
#else
// TODO: figure out how to do debugging on gdb
#define CEPUMS_ASSERT(x, ...)                                                                       \
    {                                                                                               \
        if (!(x)) {                                                                                 \
            LOG_CRITICAL("Assertation failed: {0} in {1} at {2}", __VA_ARGS__, __FILE__, __LINE__); \
            abort();                                                                                \
        }                                                                                           \
    }
#define ILLEGAL_INSTRUCTION()                                                       \
    {                                                                               \
        LOG_CRITICAL("ILLEGAL INSTRUCTION REACHED in {0}:{1}", __FILE__, __LINE__); \
        abort();                                                                    \
    }
#define UNKNOWN_INSTRUCTION()                                                      \
    {                                                                              \
        LOG_CRITICAL("Unknown instruction parsed in {0}:{1}", __FILE__, __LINE__); \
        abort();                                                                   \
    }
#define VERIFY_NOT_REACHED()                                                \
    LOG_CRITICAL("Verify not reached hit in {0}:{1}!", __FILE__, __LINE__); \
    abort()
#define TODO()                                                   \
    {                                                            \
        LOG_CRITICAL("TODO hit in {0}:{1}", __FILE__, __LINE__); \
        abort();                                                 \
    }
#define TODO_INSTRUCTION(x, y)                                                                                        \
    {                                                                                                                 \
        LOG_CRITICAL("Unimplemented instruction \"{2}\" opcode \"{3:x}h\" hit in {0}:{1}", __FILE__, __LINE__, x, y); \
        abort();                                                                                                      \
    }
#endif

#define INSTRUCTION_TRACE(...) \
    if (s_debugSpam)           \
    LOG_WARN(__VA_ARGS__)

#define BIT(x) (1 << x)
#define IS_BIT_SET(number, bit) ((number >> bit) & 1U)
#define IS_BIT_NOT_SET(number, bit) !(IS_BIT_SET(number, bit))
#define DO_PARITY_BYTE(byte) \
    byte ^= byte >> 4;       \
    byte ^= byte >> 2;       \
    byte ^= byte >> 1
#define DO_PARITY_WORD(byte) \
    byte ^= byte >> 8;       \
    byte ^= byte >> 4;       \
    byte ^= byte >> 2;       \
    byte ^= byte >> 1
#define IS_PARITY_EVEN(byteOrWord) (~byteOrWord) & 1
#define SET_BIT(byte, bit) byte |= BIT(bit)

// Extract the last 6 bits which is an opcode
#define EXTRACT_OPCODE(doubleword) (doubleword >> 26) & 0x3F; // 6 bits
#define EXTRACT_REGISTER_A(doubleword) (doubleword >> 21) & 0x1F; // 5 bits
#define EXTRACT_REGISTER_B(doubleword) (doubleword >> 16) & 0x1F; // 5 bits
#define EXTRACT_REGISTER_C(doubleword) doubleword & 0x1F; // 5 bits
#define EXTRACT_INTEGER_OPERATE_FUNCTION(doubleword) (doubleword >> 5) & 0x7F // 7 bits
#define EXTRACT_INTEGER_OPERATE_LITERAL(doubleword) (doubleword >> 13) & 0xFF // 8 bits
#define EXTRACT_INTEGER_OPERATE_IS_LITERAL_BIT(doubleword) (doubleword >> 12) & 0x1 // 1 bit
#define EXTRACT_MEMORY_DISPLACEMENT(doubleword) doubleword & 0xFFFF; // 16 bits
#define EXTRACT_BRANCH_DISPLACEMENT(doubleword) doubleword & 0x1FFFFF; // 21 bits
#define EXTRACT_PAL_FUNCTION(doubleword) doubleword & 0x03FFFFFF; // 26 bits

namespace Cepums {

// I don't know how this works
[[maybe_unused]] static uint16_t signExtendByteToWord(uint8_t byte) {
    uint16_t mask = 1u << (8 - 1);
    return (byte ^ mask) - mask;
}

[[maybe_unused]] static uint64_t signExtendBranchDisplacementToQuad(uint32_t displ) {
    uint64_t mask = 1u << (20 - 1);
    return (displ ^ mask) - mask;
}

[[maybe_unused]] static char halfByteToHexChar(uint8_t halfbyte) {
    if (halfbyte < 0xA)
        return halfbyte + 48;
    else
        return halfbyte - 10 + 65;
}

// Unique pointers aka Scoped pointers
template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename... Args>
constexpr Scope<T> createScope(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

// Shared pointers aka References
template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
constexpr Ref<T> createRef(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
} // namespace Cepums
