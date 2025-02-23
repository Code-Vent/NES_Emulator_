//
// Created by Code-Vent on 2/16/2025.
//

#include "ppu2C02.h"

PPU2C02* g_ppu = NULL;
struct Bus* g_main_bus = NULL;
struct Bus* g_graphics_bus = NULL;

#define PPUCTRL   0x2000
#define PPUMASK   0x2001
#define PPUSTATUS 0x2002
#define OAMADDR   0x2003
#define OAMDATA   0x2004
#define PPUSCROLL 0x2005
#define PPUADDR   0x2006
#define PPUDATA   0x2007
#define OAMDMA    0x4014

struct _PPU2C02 {
    union ControlSettings s_ctrl;
    union RenderingSettings s_render;
};

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
        g_ppu->s_ctrl.ppuctrl = value;
        break;
    case PPUMASK:
        g_ppu->s_render.ppumask = value;
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

PPU2C02* allocPPU(struct Bus* main_bus, struct Bus* g_bus, uint16_t start_address, uint16_t end_address) {
    assert(main_bus != NULL && g_bus != NULL);
    if (g_ppu == NULL) {
        g_ppu = malloc(sizeof(struct _PPU2C02));
        assert(g_ppu != NULL);
    }
    struct Peripheral* p = addPeripheral(main_bus, start_address, end_address);
    p->read = readPPURegisters;
    p->write = writePPURegisters;
    
    g_main_bus = main_bus;
    g_graphics_bus = g_bus;
    return g_ppu;
}

void deallocPPU(PPU2C02* ppu)
{
    free(ppu);
}

void getPPUControlSettings(struct PPUCTRLBits* bits)
{
    *bits = g_ppu->s_ctrl.bits;
}

void getPPURenderingSettings(struct PPUMASKBits* bits)
{
    *bits = g_ppu->s_render.bits;
}


