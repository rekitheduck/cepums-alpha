#include "Log.h"

int main(int argc, char** argv) {
    // Initialize the basics
    Cepums::Log::init();

    DC_CORE_INFO("Cepums-Alpha starting up ...");

    return 0;
}
