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

    // Map the QEMU PALcode rom only for now
    memory.mapROM(0xfffffc0000000000, Cepums::parseElf(std::ifstream("palcode-clipper", std::ios::binary)));

    size_t test_counter = 0;

    LOG_INFO("Cepums-Alpha starting up ...");
    while (true) {
        processor.execute(memory);
        test_counter++;

        if (test_counter > 50) {
            return 0;
        }
    }

    return 0;
}
