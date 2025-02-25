#include"nes.h"
#include<stdio.h>

#include"mappers/mapper00.h"

void setup(struct Nes* nes) {
    nes->main_bus = allocBus();
    nes->ppu_bus = allocBus();
    nes->graphics_bus = allocBus();

    //Main bus memory allocations    
    allocStack(nes->main_bus, 0x800, 256);//stack is from 0x800 to 0x8FF
    nes->cpu = allocCPU(nes->main_bus, 0x800 + 256);//The core registers begin after stack at 0x900
    nes->apu = allocAPU(nes->main_bus, 0x4000, 32);
    allocRam(nes->main_bus, 0, 0x2000);
    allocPGR_ROM(nes->main_bus, 0x4020, 0xBFE0);
    nes->ppu = allocPPU(nes->main_bus, 0x2000, 0x3FFF, nes->ppu_bus, nes->graphics_bus);
    //End of Main bus memory allocations
    
    //PPU bus memory allocations
    allocOAM(nes->ppu_bus, 0, 0x100);
    //End of PPU bus memory allocations

    //Graphics bus memory allocations        
    allocCHR_RAM(nes->graphics_bus, 0, 0x3F00);
    allocPalette(nes->graphics_bus, 0x3F00, 0x20);
    //End of Graphics bus memory allocations
   
    assert(!nes->main_bus->error);
    assert(!nes->graphics_bus->error);
    assert(!nes->ppu_bus->error);
}

void destroy(struct Nes* nes) {
    deallocBus(nes->main_bus);
    deallocBus(nes->graphics_bus);
    deallocBus(nes->ppu_bus);
    deallocCPU(nes->cpu);
    deallocAPU(nes->apu);
    deallocPPU(nes->ppu);
}
