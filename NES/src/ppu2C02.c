//
// Created by Code-Vent on 2/16/2025.
//

#include "ppu2C02.h"


#define PPUCTRL   0x2000
#define PPUMASK   0x2001
#define PPUSTATUS 0x2002
#define OAMADDR   0x2003
#define OAMDATA   0x2004
#define PPUSCROLL 0x2005
#define PPUADDR   0x2006
#define PPUDATA   0x2007
#define OAMDMA    0x4014

static struct PPURegisters core;
static PPU2C02* g_ppu;

uint16_t    core_base;
struct _PPU2C02 {
    struct Bus* graphics_bus;
    struct Bus* ppu_bus;
};

uint8_t readPPURegisters(uint16_t address) {
    switch (address & 0x2007)
    {
    case PPUSTATUS:
        return core.ppustatus.byte;
    case PPUDATA:
        return read(g_ppu->graphics_bus, core.ppuaddr.word);
    case OAMDATA:
        return read(g_ppu->ppu_bus, core.oamaddr);
    default:
        break;
    }
    return 0;
}

void writePPURegisters(uint16_t address, uint8_t value) {
    typedef struct  {
        uint8_t w : 1;
    }byte_tracker_t;
    static byte_tracker_t bt = {
        .w = 1
    };

    switch (address & 0x2007)
    {
    case PPUCTRL:
        core.ppuctrl.byte = value;
        break;
    case PPUMASK:
        core.ppumask.byte = value;
        break;
    case PPUSCROLL:
        core.ppuscroll.bytes[bt.w++] = value;
        break;
    case PPUADDR:
        core.ppuaddr.bytes[bt.w++] = value;
        break;
    case PPUDATA:
        write(g_ppu->graphics_bus, core.ppuaddr.word, value);
        break;
    case OAMADDR:
        core.oamaddr = value;
        break;
    case OAMDATA:
        write(g_ppu->ppu_bus, core.oamaddr, value);
        break;
    default:
        break;
    }
}

PPU2C02* allocPPU(struct Bus* main_bus, uint16_t start_address, uint16_t end_address, struct Bus* ppu_bus, struct Bus* g_bus) {
    assert(ppu_bus != NULL && g_bus != NULL);
    PPU2C02* g_ppu = malloc(sizeof(struct _PPU2C02));
    if (g_ppu != NULL) {
        //core_base = core_base_addr;
        g_ppu->graphics_bus = g_bus;
        g_ppu->ppu_bus = ppu_bus;
        struct Peripheral* p = addPeripheral(main_bus, start_address, end_address);
        p->read = readPPURegisters;
        p->write = writePPURegisters;
    }
    return g_ppu;
}

void deallocPPU(PPU2C02* ppu)
{
    free(ppu);
}

void getPPURegisters(struct PPURegisters* r)
{
    *r = core;
}


