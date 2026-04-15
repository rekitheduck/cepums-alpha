#include "Log.h"
#include "Memory.h"
#include "Processor/Processor.h"

#include "Utilities/ELFUtils.h"
#include <filesystem>
#include <iostream>

int main(int argc, char** argv) {
    // Initialize the basics
    Cepums::Log::init();

    namespace fs = std::filesystem;

    LOG_DEBUG("Current path is {0}", fs::current_path().string());

    Cepums::Processor processor;
    Cepums::Memory memory;

    Cepums::parseElf(std::ifstream("palcode-clipper", std::ios::binary));

    // TODO: implement a proper memory system
    // memory.mapROM(0xfffffc0000000000, Cepums::parseElf(std::ifstream("palcode-clipper", std::ios::binary)));
    // For now, RAM is hardcoded to have a few palcode instructions

    LOG_INFO("Cepums-Alpha starting up ...");
    while (true) {
        processor.execute(memory);
        LOG_DEBUG("one instruction done");
        return 0;
    }

    return 0;
}
