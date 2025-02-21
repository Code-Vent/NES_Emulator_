//
// Created by Code-Vent on 2/16/2025.
//
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include<assert.h>

#ifndef NES_PERIPHERAL_H
#define NES_PERIPHERAL_H

struct Peripheral {
    uint16_t start_address;
    uint16_t end_address;

    void (*write)(uint16_t address, uint8_t data);
    uint8_t(*read)(uint16_t address);
};

struct Peripheral* allocPeripheral(uint16_t start_address, uint16_t end_address);
bool containsAddress(struct Peripheral* p, uint16_t addr);

#endif //NES_PERIPHERAL_H
