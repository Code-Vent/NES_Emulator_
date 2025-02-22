#include"nes.h"


void setup(struct Nes* nes) {
    nes->main_bus = allocBus();
    allocRam(nes->main_bus, 0, 0x800);
    allocStack(nes->main_bus, 0x1000, 256);
    allocCartridge(nes->main_bus, 0x4020, 0xBFE0);
    nes->cpu = allocCPU(nes->main_bus, 0x1000);
    nes->apu = allocAPU(nes->main_bus, 0x4000, 64);

    nes->graphics_bus = allocBus();
    allocVRam(nes->graphics_bus, 0x2000, 0x800);

    allocPictureProcessingUnit(&nes->ppu, nes->main_bus, 0x2000, 0x3FFF);
    addGraphicsBusToPPU(&nes->ppu, nes->graphics_bus);

    for (uint16_t i = 0; i < 0x800; ++i) {
        uint16_t address = 0x2000 + i;
        printf("%x ", read(nes->graphics_bus, address));
    }
}

void destroy(struct Nes* nes) {
    deallocBus(nes->main_bus);
    deallocBus(nes->graphics_bus);
    deallocCPU(nes->cpu);
    deallocAPU(nes->apu);
}
