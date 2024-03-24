#pragma once

#include <cstdint>

namespace Cepums {

class Processor {
   public:
    Processor() { reset(); }

    void reset();
    void execute();

   private:
};
} // namespace Cepums
