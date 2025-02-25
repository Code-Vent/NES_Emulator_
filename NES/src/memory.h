//
// Created by Code-Vent on 2/16/2025.
//

#include"bus.h"

#ifndef NES_MEMORY_H
#define NES_MEMORY_H

void allocRam(struct Bus* bus, uint16_t start_address, uint16_t size);
void allocCHR_RAM(struct Bus* bus, uint16_t start_address, uint16_t size);
void allocPGR_ROM(struct Bus* bus, uint16_t start_address, uint16_t size);
void allocStack(struct Bus* bus, uint16_t start_address, uint16_t size);
void allocOAM(struct Bus* bus, uint16_t start_address, uint16_t size);
void allocPalette(struct Bus* bus, uint16_t start_address, uint16_t size);


#endif //NES_MEMORY_H
