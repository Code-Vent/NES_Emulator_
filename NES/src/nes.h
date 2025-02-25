#ifndef NES__H
#define NES__H

#include "bus.h"
#include "memory.h"
#include "cpu6502.h"
#include "ppu2C02.h"
#include"apu2A03.h"
//#include"raylib/raylib.h"

struct Nes {
    CPU6502* cpu;
    APU2A03* apu;
    PPU2C02* ppu;
    struct Bus* main_bus;
    struct Bus* graphics_bus;
    struct Bus* ppu_bus;    
};

void setup(struct Nes* nes);
void destroy(struct Nes* nes);


#endif //NES_H