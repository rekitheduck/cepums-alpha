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

    Cepums::parseElf(std::ifstream("palcode-clipper", std::ios::binary));

    return 0;

    Cepums::Processor processor;
    Cepums::Memory memory;

    LOG_INFO("Cepums-Alpha starting up ...");
    while (true) {
        processor.execute(memory);
    }

    return 0;
}
