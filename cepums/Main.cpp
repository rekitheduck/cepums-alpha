#include "Log.h"
#include "Processor/Processor.h"

int main(int argc, char** argv) {
    // Initialize the basics
    Cepums::Log::init();

    Cepums::Processor processor;

    DC_CORE_INFO("Cepums-Alpha starting up ...");

    return 0;
}
