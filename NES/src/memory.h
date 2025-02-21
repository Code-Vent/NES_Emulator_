//
// Created by Code-Vent on 2/16/2025.
//

#include"bus.h"

#ifndef NES_MEMORY_H
#define NES_MEMORY_H

void allocRam(struct Bus* bus, uint16_t start_address, uint16_t size);
void allocVRam(struct Bus* bus, uint16_t start_address, uint16_t size);
void allocStack(struct Bus* bus, uint16_t start_address, uint16_t size);
void allocCartridge(struct Bus* bus, uint16_t start_address, uint16_t size);
uint8_t directReadCartridge(uint16_t address);
void directWriteCartridge(uint16_t address, uint8_t value);

#endif //NES_MEMORY_H
