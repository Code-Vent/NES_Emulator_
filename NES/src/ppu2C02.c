//
// Created by Code-Vent on 2/16/2025.
//

#include "ppu2C02.h"

struct PPU2C02* g_ppu = NULL;
struct Bus* g_main_bus = NULL;

#define PPUCTRL   0x2000
#define PPUMASK   0x2001
#define PPUSTATUS 0x2002
#define OAMADDR   0x2003
#define OAMDATA   0x2004
#define PPUSCROLL 0x2005
#define PPUADDR   0x2006
#define PPUDATA   0x2007
#define OAMDMA    0x4014

uint8_t readPPURegisters(uint16_t address) {
    switch (address & 0x2007)
    {
    case PPUSTATUS:
        break;
    case PPUDATA:
        break;
    case OAMDATA:
        break;
    default:
        break;
    }
    return 0;
}

void writePPURegisters(uint16_t address, uint8_t value) {
    switch (address & 0x2007)
    {
    case PPUCTRL:
        break;
    case PPUMASK:
        break;
    case PPUSCROLL:
        break;
    case PPUADDR:
        break;
    case PPUDATA:
        break;
    case OAMADDR:
        break;
    case OAMDATA:
        break;
    default:
        break;
    }
}

void addGraphicsBusToPPU(struct PPU2C02* ppu, struct Bus* g_bus) {
    assert(ppu != NULL);
    ppu->graphics_bus = g_bus;
}

void allocPictureProcessingUnit(struct PPU2C02* ppu, struct Bus* main_bus, uint16_t start_address, uint16_t end_address) {
    assert(main_bus != NULL);
    struct Peripheral* p = addPeripheral(main_bus, start_address, end_address);
    p->read = readPPURegisters;
    p->write = writePPURegisters;
    g_ppu = ppu;
    g_main_bus = main_bus;
}
