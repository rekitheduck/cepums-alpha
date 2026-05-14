#include "ELFUtils.h"

#include "Log.h"

#define DEBUG_LOG_ELF 0

namespace Cepums {

std::vector<uint8_t> parseElf(std::ifstream file) {
    std::vector<uint8_t> output;
    if (file.fail()) {
        LOG_ERROR("palcode not found :(");
        return output;
    }

    ElfHeader header;
    file.read((char*)&header, sizeof(ElfHeader));

    if (header.e_ident[0] != 0x7f || header.e_ident[1] != 'E' || header.e_ident[2] != 'L' || header.e_ident[3] != 'F') {
        LOG_ERROR("Elf not valid, cannot continue");
        return output;
    }

    std::string ident_hex;
    for (size_t i = 0; i < 16; i++) {
        ident_hex += std::format("{:0>2x} ", header.e_ident[i]);
    }

#if DEBUG_LOG_ELF
    LOG_DEBUG("e_ident: {0}", ident_hex);

    LOG_DEBUG("e_type: {0:x}h", header.e_type);
    LOG_DEBUG("e_machine: {0:x}h", header.e_machine);
    LOG_DEBUG("e_version: {0:x}h", header.e_version);
    LOG_DEBUG("e_entry: {0:x}h", header.e_entry);
    LOG_DEBUG("e_phoff: {0:x}h", header.e_phoff);
    LOG_DEBUG("e_shoff: {0:x}h", header.e_shoff);
    LOG_DEBUG("e_flags: {0:x}h", header.e_flags);
    LOG_DEBUG("e_ehsize: {0:x}h", header.e_ehsize);
    LOG_DEBUG("e_phentsize: {0:x}h", header.e_phentsize);
    LOG_DEBUG("e_phnum: {0:x}h", header.e_phnum);
    LOG_DEBUG("e_shentsize: {0:x}h", header.e_shentsize);
    LOG_DEBUG("e_shnum: {0:x}h", header.e_shnum);
    LOG_DEBUG("e_shstrndx: {0:x}h", header.e_shstrndx);
#endif

    // Some safety checks
    CEPUMS_ASSERT(header.e_ident[4] == 2, "Not a 64-bit ELF!");
    CEPUMS_ASSERT(header.e_ident[5] == 1, "Not a LE ELF!");
    CEPUMS_ASSERT(header.e_type == 2, "Not an ET_EXEC ELF!");

    // Load program headers
    std::vector<ElfProgramHeader> program_headers;
    file.seekg(header.e_phoff);
    for (size_t i = 0; i < header.e_phnum; i++) {
        auto& prog_head = program_headers.emplace_back();
        file.read((char*)&prog_head, sizeof(ElfProgramHeader));
    }

#if DEBUG_LOG_ELF
    LOG_DEBUG("Found {0} program headers", program_headers.size());
    for (const auto& h : program_headers) {
        LOG_INFO("Start of new program header!");
        LOG_DEBUG("p_type: {0:x}h", h.p_type);
        LOG_DEBUG("p_flags: {0:x}h", h.p_flags);
        LOG_DEBUG("p_offset: {0:x}h", h.p_offset);
        LOG_DEBUG("p_vaddr: {0:x}h", h.p_vaddr);
        LOG_DEBUG("p_addr: {0:x}h", h.p_addr);
        LOG_DEBUG("p_filesz: {0:x}h", h.p_filesz);
        LOG_DEBUG("p_memsz: {0:x}h", h.p_memsz);
        LOG_DEBUG("p_align: {0:x}h", h.p_align);
    }
#endif

    // There's a second header of the type PT_GNU_STACK which I think we can just ignore ^_^
    output.resize(program_headers[0].p_memsz, 0); // expand and 0-fill the entire memory range

    file.seekg(program_headers[0].p_offset);
    file.read((char*)output.data(), program_headers[0].p_filesz);

#if DEBUG_LOG_ELF
    for (size_t i = 0; i < 50; i++) {
        LOG_DEBUG("{0}, {1:x}h", i, output[i]);
    }
#endif

    return output;
}

} // namespace Cepums
