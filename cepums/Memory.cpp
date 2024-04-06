#include "Memory.h"
#include "cepumspch.h"

namespace Cepums {
Memory::Memory() {
    // Temporary test code in memory
    m_ram.reserve(50000);

    /*
    fffffc0000000000 <.text>:
    fffffc0000000000:	00 00 a0 c3 	br	gp,0xfffffc0000000004
    fffffc0000000004:	01 00 bd 27 	ldah	gp,1(gp)
    fffffc0000000008:	4c 2f bd 23 	lda	gp,12108(gp)
    fffffc000000000c:	32 00 fd 77 	pal1d	0x3fd0032
    fffffc0000000010:	07 00 3f 20 	lda	t0,7
    fffffc0000000014:	00 00 e1 77 	pal1d	0x3e10000
    */
    m_ram = {0x0,  0x0,  0xa0, 0x01, 0x00, 0xbd, 0x27, 0x4c, 0x2f, 0xbd, 0x23, 0x32,
             0x00, 0xfd, 0x77, 0x07, 0x00, 0x3f, 0x20, 0x00, 0x00, 0xe1, 0x77};
}

uint64_t Memory::readQuad(uint64_t address) {
    return 0;
}

} // namespace Cepums
