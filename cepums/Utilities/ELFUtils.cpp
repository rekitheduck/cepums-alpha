#include "ELFUtils.h"

#include "Log.h"

namespace Cepums {

void parseElf(std::ifstream file) {
    if (file.fail()) {
        LOG_ERROR("palcode not found :(");
        return;
    }

    ElfHeader header;
    file.read((char*)&header, sizeof(ElfHeader));

    LOG_INFO("ELF Header parsed");

    std::string ident_hex;
    for (size_t i = 0; i < 16; i++) {
        ident_hex += std::format("{:0>2x} ", header.e_ident[i]);
    }

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
}

} // namespace Cepums
