//
// Created by Code-Vent on 2/16/2025.

#include<stdint.h>
#include "bus.h"

#ifndef NES_PICTURE_PROCESSING_UNIT_H
#define NES_PICTURE_PROCESSING_UNIT_H

struct PPU2C02 {
    struct Bus* graphics_bus;
};

void addGraphicsBusToPPU(struct PPU2C02* ppu, struct Bus* g_bus);
void allocPictureProcessingUnit(struct PPU2C02* ppu, struct Bus* main_bus, uint16_t start_address, uint16_t end_address);

#endif //NES_PICTURE_PROCESSING_UNIT_H
