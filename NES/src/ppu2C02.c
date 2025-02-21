//
// Created by Tao on 2/16/2025.
//

#include "ppu2C02.h"

struct PPU2C02* g_ppu = NULL;
struct Bus* g_main_bus = NULL;

uint8_t readPPU(uint16_t address) {
    return read(g_ppu->graphics_bus, 0x2000);
}

void writePPU(uint16_t address, uint8_t value) {

}

void addGraphicsBusToPPU(struct PPU2C02* ppu, struct Bus* g_bus) {
    assert(ppu != NULL);
    ppu->graphics_bus = g_bus;
}

void allocPictureProcessingUnit(struct PPU2C02* ppu, struct Bus* main_bus, uint16_t start_address, uint16_t end_address) {
    assert(main_bus != NULL);
    struct Peripheral* p = addPeripheral(main_bus, start_address, end_address);
    p->read = readPPU;
    p->write = writePPU;
    g_ppu = ppu;
    g_main_bus = main_bus;
}
