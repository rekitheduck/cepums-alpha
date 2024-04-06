#include "Log.h"
#include "Memory.h"
#include "Processor/Processor.h"

int main(int argc, char** argv) {
    // Initialize the basics
    Cepums::Log::init();

    Cepums::Processor processor;
    Cepums::Memory memory;

    LOG_INFO("Cepums-Alpha starting up ...");
    while (true) {
        processor.execute(memory);
    }

    return 0;
}
